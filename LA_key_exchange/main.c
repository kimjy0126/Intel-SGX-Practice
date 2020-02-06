#include <stdio.h>
#include "Sender/Sender_u.h"
#include "Receiver/Receiver_u.h"
#include "sgx_eid.h"
#include "sgx_urts.h"
#include "sgx_error.h"

int main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    sgx_status_t ret;
    int retval;

    sgx_enclave_id_t eid_sender;
    sgx_enclave_id_t eid_recver;

    /* load enclaves */
    if ((ret = sgx_create_enclave("Sender.signed.so", SGX_DEBUG_FLAG, NULL, NULL, \
                    &eid_sender, NULL)) \
            != SGX_SUCCESS) {
        printf("sgx_create_enclave failed: Failed to load Sender.signed.so (ret: 0x%x)\n", \
                ret);
        return -1;
    }
    if ((ret = sgx_create_enclave("Receiver.signed.so", SGX_DEBUG_FLAG, NULL, NULL, \
                    &eid_recver, NULL)) \
            != SGX_SUCCESS) {
        printf("sgx_create_enclave failed: Failed to load Receiver.signed.so (ret: 0x%x)\n", \
                ret);
        return -1;
    }

    /* create session between Sender and Receiver */
    if ((ret = Sender_initialize_session(eid_sender, &retval, eid_sender, eid_recver)) \
            != SGX_SUCCESS) {
        printf("Sender_initialize_session failed: Failed to initialize session (ret: 0x%x)\n", \
                ret);
        return -1;
    } else if (retval != 0) {
        printf("Sender_initialize_session failed: retval is not zero\n");
        return -1;
    } else {
        printf("*** Successfully initialized session between Sender and Receiver ***\n");
    }
    return 0;
}
