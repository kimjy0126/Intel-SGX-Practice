#include <stdio.h>
#include "sgx_eid.h"
#include "sgx_urts.h"
#include "sgx_dh.h"
#include "Sender/Sender_u.h"
#include "Receiver/Receiver_u.h"

int session_request_ocall(sgx_enclave_id_t eid_src, sgx_enclave_id_t eid_dst, \
        sgx_dh_msg1_t* dh_msg1, uint32_t* session_id)
{
    sgx_status_t ret;
    int retval;

    /* Step 3 */
    if ((ret = Receiver_session_request(eid_dst, &retval, eid_src, dh_msg1, session_id)) \
            != SGX_SUCCESS) {
        printf("Receiver_session_request failed: Failed to request session (ret: 0x%x)\n", \
                ret);
        return -1;
    } else if (retval != 0) {
        printf("Receiver_session_request failed: retval is not zero\n");
        return -1;
    }

    return 0;
}

int exchange_report_ocall(sgx_enclave_id_t eid_src, sgx_enclave_id_t eid_dst, \
        sgx_dh_msg2_t* dh_msg2, sgx_dh_msg3_t* dh_msg3, uint32_t session_id)
{
    sgx_status_t ret;
    int retval;

    /* Step 9 */
    if ((ret = Receiver_exchange_report(eid_dst, &retval, eid_src, \
                    dh_msg2, dh_msg3, session_id)) != SGX_SUCCESS) {
        printf("Receiver_exchange_report failed: Failed to exchange report (ret: 0x%x)\n", \
                ret);
        return -1;
    } else if (retval != 0) {
        printf("Receiver_exchange_report failed: retval is not zero\n");
        return -1;
    }
    
    return 0;
}
