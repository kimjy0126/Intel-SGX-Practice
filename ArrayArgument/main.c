#include <stdio.h>
#include "sgx_urts.h"
#include "sgx_eid.h" /* sgx_enclave_id_t */
#include "sgx_error.h" /* sgx_status_t */
#include "enclave/enclave_u.h"

#define ENCLAVE_FILENAME "enclave.signed.so"

sgx_enclave_id_t global_eid = 0;

void ocall_print(char ch)
{
//    printf("Start of ocall_print\n");
    printf("%c\n", ch);
//    printf("End of ocall_print\n");
}

int main(void)
{
    printf("Start of main\n");

    char pw[11];

    printf("Enter your password (exactly 10 characters): ");
    fgets(pw, 11, stdin);

	if (sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, NULL, NULL, &global_eid, NULL) != SGX_SUCCESS) {
        printf("sgx_create_enclave failed\n");
		return -1;
	}

    sgx_status_t ret = ecall_reverse_pw(global_eid, pw);
    if (ret != SGX_SUCCESS) {
        printf("WTF!\n");
    }

    printf("Processed password: %s\n", pw);

	if (sgx_destroy_enclave(global_eid) != SGX_SUCCESS) {
        printf("sgx_destroy_enclave failed\n");
		return -1;
	}

    printf("End of main\n");
	return 0;
}
