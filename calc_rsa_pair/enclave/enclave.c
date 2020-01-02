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

    sgx_rsa3072_key_t p_key;
    p_key.e[0] = 17;
    p_key.e[1] = 0;
    p_key.e[2] = 0;
    p_key.e[3] = 0;
    ret = sgx_create_rsa_key_pair(SGX_RSA3072_KEY_SIZE, SGX_RSA3072_PUB_EXP_SIZE, \
            (unsigned char*)p_key.mod, (unsigned char*)p_key.d, \
            (unsigned char*)p_key.e, (unsigned char*)p, (unsigned char*)q, (unsigned char*)dmp1,
            (unsigned char*)dmq1, (unsigned char*)iqmp);

    if (ret == SGX_SUCCESS) {
        ocall_print("Successfully created rsa key pair!\n");
        ocall_print("n:\n");
        for (int i = 0; i < 384; i++) {
            ocall_printd(p_key.mod[i]);
            if ((i + 1) % 4 == 0) ocall_print(" ");
            if ((i + 1) % 32 == 0) ocall_print("\n");
        }
        ocall_print("e:\n");
        for (int i = 0; i < 4; i++) {
            ocall_printd(p_key.e[i]);
        }
        ocall_print("\n");
        ocall_print("d:\n");
        for (int i = 0; i < 384; i++) {
            ocall_printd(p_key.d[i]);
            if ((i + 1) % 4 == 0) ocall_print(" ");
            if ((i + 1) % 32 == 0) ocall_print("\n");
        }
    }

    void* new_pri_key2;
    ret = sgx_create_rsa_priv2_key(SGX_RSA3072_KEY_SIZE, SGX_RSA3072_PUB_EXP_SIZE, \
            (unsigned char*)p_key.e, (unsigned char*)p, (unsigned char*)q, \
            (unsigned char*)dmp1, (unsigned char*)dmq1, (unsigned char*)iqmp, \
            (void**)&new_pri_key2);
    if (ret == SGX_SUCCESS) ocall_print("Converted priv key\n");
    void* new_pub_key1;
    ret = sgx_create_rsa_pub1_key(SGX_RSA3072_KEY_SIZE, SGX_RSA3072_PUB_EXP_SIZE, \
            (unsigned char*)p_key.mod, (unsigned char*)p_key.e, \
            (void**)&new_pub_key1);
    if (ret == SGX_SUCCESS) ocall_print("Converted pub key\n");

    uint8_t p_data[] = { 0x12, 0x34, 0x56, 0x78 };
    uint8_t pout[384] = {0, };
    size_t pout_size = 384;
    ret = sgx_rsa_pub_encrypt_sha256(new_pub_key1, (unsigned char*)pout, &pout_size, \
            (unsigned char*)p_data, 4);
    if (ret == SGX_SUCCESS) ocall_print("Encrypted!\n");

    uint8_t pout2[384] = {0, };
    size_t pout2_size = 384;
    ret = sgx_rsa_priv_decrypt_sha256(new_pri_key2, (unsigned char*)pout2, &pout2_size, \
            (unsigned char*)pout, 384);
    if (ret == SGX_SUCCESS) ocall_print("Decrypted!\n");

    for (int i = 0; i < 384; i++) {
        ocall_printd(pout2[i]);
        if ((i + 1) % 4 == 0) ocall_print(" ");
        if ((i + 1) % 32 == 0) ocall_print("\n");
    }
}
