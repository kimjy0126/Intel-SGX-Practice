#SGX_SDK = /home/kimjy/Desktop/linux-sgx/linux/installer/bin/sgxsdk
SGX_LIBRARY_PATH = $(SGX_SDK)/lib64
SGX_SDK_INCLUDE = $(SGX_SDK)/include

######### SIMULATION MODE ###########
SGX_TSERVICE = sgx_tservice
SGX_URTS = sgx_urts
SGX_UAE_SERVICE = sgx_uae_service

Enclave_Link_Flags := -Wl,-z,relro,-z,now,-z,noexecstack \
    -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_LIBRARY_PATH) \
    -Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive \
    -Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tcrypto -l$(SGX_TSERVICE) -Wl,--end-group \
    -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
    -Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
    -Wl,--defsym,__ImageBase=0 -Wl,--gc-sections   \
   # -Wl,--version-script=enclave.lds

enclave/enclave.o:
	gcc -I$(SGX_SDK_INCLUDE) -o enclave/enclave.o -c enclave/enclave.c

enclave/enclave_u.h:	enclave/enclave.edl
	cd enclave && sgx_edger8r --untrusted ../enclave/enclave.edl

enclave/enclave_u.c:	enclave/enclave_u.h

enclave/enclave_u.o:	enclave/enclave_u.c
	gcc -I$(SGX_SDK_INCLUDE) -I$(SGX_SDK_INCLUDE)/tlibc -o enclave/enclave_u.o -c enclave/enclave_u.c

enclave/enclave_t.h:	enclave/enclave.edl
	cd enclave && sgx_edger8r --trusted ../enclave/enclave.edl

enclave/enclave_t.c:	enclave/enclave_t.h

enclave/enclave_t.o:
	gcc -I$(SGX_SDK_INCLUDE) -I$(SGX_SDK_INCLUDE)/tlibc -o enclave/enclave_t.o -c enclave/enclave_t.c

enclave.so:		enclave/enclave_t.o enclave/enclave.o
	gcc enclave/enclave_t.o enclave/enclave.o -o enclave.so $(Enclave_Link_Flags)

enclave.signed.so: enclave.so
	sgx_sign sign -enclave enclave.so -config enclave/enclave.config.xml -out enclave.signed.so -key enclave/enclave_private.pem

main.o:
	gcc -o main.o -I $(SGX_SDK_INCLUDE) -c main.c

main: enclave/enclave_u.o main.o
	gcc -L $(SGX_LIBRARY_PATH) -I $(SGX_SDK_INCLUDE) -o main main.o enclave/enclave_u.o -l$(SGX_URTS) -lpthread -l$(SGX_UAE_SERVICE)

clean:
	rm -f enclave/enclave_t*
	rm -f enclave/enclave_u*
	rm -f enclave/enclave.o
	rm -f main.o
	rm -f main
	rm -f enclave.so
	rm -f enclave.signed.so
