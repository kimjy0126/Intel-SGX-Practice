#include "enclave_t.h"
#include "sgx_tcrypto.h"

void ecall_manage_rsa(void)
{
    sgx_status_t ret;
    unsigned int n[384 / 4];
    unsigned int e[4 / 4];
    unsigned int d[384 / 4];
    unsigned int p[192 / 4];
    unsigned int q[192 / 4];
    unsigned int dmp1[192 / 4];
    unsigned int dmq1[192 / 4];
    unsigned int iqmp[192 / 4];

    e[0] = 17;
    ret = sgx_create_rsa_key_pair(384, 4, (unsigned char*)n, (unsigned char*)d, \
            (unsigned char*)e, (unsigned char*)p, (unsigned char*)q, (unsigned char*)dmp1,
            (unsigned char*)dmq1, (unsigned char*)iqmp);

    if (ret == SGX_SUCCESS) {
        ocall_print("Successfully created rsa key pair!\n");
        ocall_print("n:\n");
        for (int i = 0; i < 384 / 4; i++) {
            ocall_printd(n[i]);
            ocall_print(" ");
            if ((i + 1) % 8 == 0) ocall_print("\n");
        }
        ocall_print("e:\n");
        for (int i = 0; i < 4 / 4; i++) {
            ocall_printd(e[i]);
            ocall_print(" ");
        }
        ocall_print("\n");
        ocall_print("d:\n");
        for (int i = 0; i < 384 / 4; i++) {
            ocall_printd(d[i]);
            ocall_print(" ");
            if ((i + 1) % 8 == 0) ocall_print("\n");
        }
    }
    else ocall_print("NO\n");
}
