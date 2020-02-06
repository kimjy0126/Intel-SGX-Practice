#include "sgx_eid.h"
#include "sgx_dh.h"
#include "Sender_t.h"

static sgx_dh_session_t sgx_dh_session;
static sgx_key_128bit_t aek;
static sgx_dh_session_enclave_identity_t responder_identity;

int initialize_session(sgx_enclave_id_t eid_src, sgx_enclave_id_t eid_dst)
{
    return create_session(eid_src, eid_dst, &sgx_dh_session);
}

int create_session(sgx_enclave_id_t eid_src, sgx_enclave_id_t eid_dst, \
        sgx_dh_session_t* session_info)
{
    sgx_status_t ret;
    int retval;

    /* Step 1 */
    if ((ret = sgx_dh_init_session(SGX_DH_SESSION_INITIATOR, session_info)) \
            != SGX_SUCCESS) {
        ocall_printe("sgx_dh_init_session failed: Failed to call lib function", ret);
        return -1;
    }

    /* Step 2 */
    sgx_dh_msg1_t dh_msg1;
    uint32_t session_id;
    if ((ret = session_request_ocall(&retval, eid_src, eid_dst, &dh_msg1, &session_id)) \
            != SGX_SUCCESS) {
        ocall_printe("session_request_ocall failed: OCALL failed", ret);
        return -1;
    } else if (retval != 0) {
        ocall_prints("session_request_ocall failed: retval is not zero\n");
        return -1;
    }

    /* Step 7 */
    sgx_dh_msg2_t dh_msg2;
    if ((ret = sgx_dh_initiator_proc_msg1(&dh_msg1, &dh_msg2, session_info)) != SGX_SUCCESS) {
        ocall_printe("sgx_dh_initiator_proc_msg1 failed: Failed to call lib function", ret);
        return -1;
    }

    /* Step 8 */
    sgx_dh_msg3_t dh_msg3;
    if ((ret = exchange_report_ocall(&retval, eid_src, eid_dst, \
                    &dh_msg2, &dh_msg3, session_id)) != SGX_SUCCESS) {
        ocall_printe("exchange_report_ocall failed: Failed to call lib function", ret);
        return -1;
    } else if (retval != 0) {
        ocall_prints("exchange_report_ocall failed: retval is not zero\n");
        return -1;
    }

    /* Step 12 */
    if ((ret = sgx_dh_initiator_proc_msg3(&dh_msg3, session_info, &aek, &responder_identity)) \
            != SGX_SUCCESS) {
        ocall_printe("sgx_dh_initiator_proc_msg3 failed: Failed to call lib function", ret);
        return -1;
    }

    int tmp = sizeof(sgx_key_128bit_t);
    ocall_prints("Sender AEK: \n");
    for (int i = 0; i < tmp; i++) {
        ocall_printx(*((char*)aek + i));
        ocall_prints(" ");
    }
    ocall_prints("\n");

    return 0;
}
