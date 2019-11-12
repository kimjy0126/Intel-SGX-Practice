echo -e "\033[31m"Removing files..."\033[0m"
make clean
echo -e "\033[31m"Removed files..."\033[0m"

echo -e "\033[31m"enclave_u.h"\033[0m"
make enclave/enclave_u.h
echo -e "\033[31m"enclave_u.o"\033[0m"
make enclave/enclave_u.o
echo -e "\033[31m"main.o"\033[0m"
make main.o
echo -e "\033[31m"main"\033[0m"
make main
echo -e "\033[31m"enclave_t.h"\033[0m"
make enclave/enclave_t.h
echo -e "\033[31m"enclave_t.o"\033[0m"
make enclave/enclave_t.o
echo -e "\033[31m"enclave.o"\033[0m"
make enclave/enclave.o
echo -e "\033[31m"enclave.so"\033[0m"
make enclave.so
echo -e "\033[31m"enclave.signed.so"\033[0m"
make enclave.signed.so
