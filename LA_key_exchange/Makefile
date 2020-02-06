SGX_SDK ?= /opt/intel/sgxsdk/
SGX_MODE ?= HW

SGX_LIBRARY_PATH = $(SGX_SDK)/lib64
SGX_SDK_INCLUDE = $(SGX_SDK)/include
ifeq ($(SGX_MODE), HW)
	SGX_TSERVICE = sgx_tservice
	SGX_URTS = sgx_urts
	SGX_TRTS = sgx_trts
	SGX_UAE_SERVICE = sgx_uae_service
else
	SGX_TSERVICE = sgx_tservice_sim
	SGX_URTS = sgx_urts_sim
	SGX_TRTS = sgx_trts_sim
	SGX_UAE_SERVICE = sgx_uae_service_sim
endif

ENCLAVE_LINK_FLAGS := -Wl,-z,relro,-z,now,-z,noexecstack \
	-Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_LIBRARY_PATH) \
	-Wl,--whole-archive -l$(SGX_TRTS) -Wl,--no-whole-archive \
	-Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tcrypto -l$(SGX_TSERVICE) \
	-Wl,--end-group -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
        -Wl,-pie,-eenclave_entry -Wl,--export-dynamic -Wl,--defsym,__ImageBase=0 \
	-Wl,--gc-sections


all: Sender.signed.so Receiver.signed.so main

ocall_print.o:	ocall_print.c
	gcc -o ocall_print.o -c ocall_print.c
untrusted.o:	untrusted.c
	gcc -o untrusted.o -I$(SGX_SDK_INCLUDE) -c untrusted.c

### Sender ###
Sender/Sender.o:	Sender/Sender.c
	gcc -o Sender/Sender.o -I$(SGX_SDK_INCLUDE) -c Sender/Sender.c

Sender/Sender_u.h:	Sender/Sender.edl
	cd Sender && sgx_edger8r --use-prefix --untrusted Sender.edl

Sender/Sender_u.c:	Sender/Sender_u.h

Sender/Sender_u.o:	Sender/Sender_u.c
	gcc -o Sender/Sender_u.o -I$(SGX_SDK_INCLUDE) -I$(SGX_SDK_INCLUDE)/tlibc -c Sender/Sender_u.c

Sender/Sender_t.h:	Sender/Sender.edl
	cd Sender && sgx_edger8r --use-prefix --trusted Sender.edl

Sender/Sender_t.c:	Sender/Sender_t.h

Sender/Sender_t.o:	Sender/Sender_t.c
	gcc -o Sender/Sender_t.o -I$(SGX_SDK_INCLUDE) -I$(SGX_SDK_INCLUDE)/tlibc -c Sender/Sender_t.c

Sender.so:	Sender/Sender_t.o Sender/Sender.o
	gcc -o Sender.so Sender/Sender_t.o Sender/Sender.o $(ENCLAVE_LINK_FLAGS)

Sender.signed.so:	Sender.so
	sgx_sign sign -enclave Sender.so -out Sender.signed.so -key Sender/priv_sender.pem

### Receiver ###
Receiver/Receiver.o:	Receiver/Receiver.c
	gcc -o Receiver/Receiver.o -I$(SGX_SDK_INCLUDE) -c Receiver/Receiver.c

Receiver/Receiver_u.h:	Receiver/Receiver.edl
	cd Receiver && sgx_edger8r --use-prefix --untrusted Receiver.edl

Receiver/Receiver_u.c:	Receiver/Receiver_u.h

Receiver/Receiver_u.o:	Receiver/Receiver_u.c
	gcc -o Receiver/Receiver_u.o -I$(SGX_SDK_INCLUDE) -I$(SGX_SDK_INCLUDE)/tlibc -c Receiver/Receiver_u.c

Receiver/Receiver_t.h:	Receiver/Receiver.edl
	cd Receiver && sgx_edger8r --use-prefix --trusted Receiver.edl

Receiver/Receiver_t.c:	Receiver/Receiver_t.h

Receiver/Receiver_t.o:	Receiver/Receiver_t.c
	gcc -o Receiver/Receiver_t.o -I$(SGX_SDK_INCLUDE) -I$(SGX_SDK_INCLUDE)/tlibc -c Receiver/Receiver_t.c

Receiver.so:	Receiver/Receiver_t.o Receiver/Receiver.o
	gcc -o Receiver.so Receiver/Receiver_t.o Receiver/Receiver.o $(ENCLAVE_LINK_FLAGS)

Receiver.signed.so:	Receiver.so
	sgx_sign sign -enclave Receiver.so -out Receiver.signed.so -key Receiver/priv_receiver.pem

### main ###
main.o:	main.c
	gcc -o main.o -I$(SGX_SDK_INCLUDE) -c main.c

main:	Sender/Sender_u.o Receiver/Receiver_u.o main.o ocall_print.o untrusted.o
	gcc -o main -L$(SGX_LIBRARY_PATH) -I$(SGX_SDK_INCLUDE) main.o ocall_print.o untrusted.o Sender/Sender_u.o Receiver/Receiver_u.o -l$(SGX_URTS) -lpthread -l$(SGX_UAE_SERVICE)

clean:
	rm -f Sender/Sender_t*
	rm -f Sender/Sender_u*
	rm -f Receiver/Receiver_t*
	rm -f Receiver/Receiver_u*
	rm -f *.so
	rm -f *.o
	rm -f main
