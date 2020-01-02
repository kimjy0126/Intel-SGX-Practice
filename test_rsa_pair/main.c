#include <stdio.h>
#include "sgx_urts.h"
#include "sgx_eid.h"
#include "sgx_error.h"
#include "enclave/enclave_u.h"

void ocall_print(char* str)
{
    printf("%s", str);
}

void ocall_printd(uint8_t n)
{
    printf("%02x", (unsigned int)n);
}

int main(int argc, const char **argv)
{
    sgx_enclave_id_t eid = 0;
    sgx_create_enclave("enclave.signed.so", SGX_DEBUG_FLAG, NULL, NULL, &eid, NULL);
    ecall_manage_rsa(eid);
    sgx_destroy_enclave(eid);

    return 0;
}
