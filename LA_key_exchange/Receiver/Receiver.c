#include "sgx_eid.h"
#include "sgx_dh.h"
#include "Receiver_t.h"

static sgx_dh_session_t sgx_dh_session;
static sgx_key_128bit_t aek;
static sgx_dh_session_enclave_identity_t initiator_identity;

int session_request(sgx_enclave_id_t eid_src, sgx_dh_msg1_t* dh_msg1, uint32_t* session_id)
{
    sgx_status_t ret;

    /* Step 4 */
    if ((ret = sgx_dh_init_session(SGX_DH_SESSION_RESPONDER, &sgx_dh_session)) \
            != SGX_SUCCESS) {
        ocall_printe("sgx_dh_init_session failed: Failed to call lib function", ret);
        return -1; 
    }

    /* generate session id */
    *session_id = 1;    // just use fixed value, because there is only one session

    /* Step 5 */
    if ((ret = sgx_dh_responder_gen_msg1(dh_msg1, &sgx_dh_session)) != SGX_SUCCESS) {
        ocall_printe("sgx_dh_responder_gen_msg1 failed: Failed to call lib function", ret);
        return -1;
    }
    /*session_info.status = 1;    // IN_PROGRESS
    int tmp = sizeof(sgx_dh_session_t);
    for (int i = 0; i < tmp; i++) {
        (char*)(&session_info->in_progress.dh_session) + i = (char*)&sgx_dh_session + i;
    }*/

    return 0;
}

int exchange_report(sgx_enclave_id_t eid_src, sgx_dh_msg2_t* dh_msg2, \
        sgx_dh_msg3_t* dh_msg3, uint32_t session_id)
{
    sgx_status_t ret;

    /* Step 10 */
    dh_msg3->msg3_body.additional_prop_length = 0;
/*    int tmp = sizeof(sgx_dh_session_t);
    for (int i = 0; i < tmp; i++) {
        (char*)&sgx_dh_session + i = (char*)(&session_info->in_progress.dh_session) + i;
    }*/
//    sgx_key_128bit_t dh_aek;
    int tmp = sizeof(sgx_key_128bit_t);
    for (int i = 0; i < tmp; i++) {
        *((char*)aek + i) = 0;
    }
    if ((ret = sgx_dh_responder_proc_msg2(dh_msg2, dh_msg3, &sgx_dh_session, \
                        &aek, &initiator_identity)) != SGX_SUCCESS) {
        ocall_printe("sgx_dh_responder_proc_msg2 failed: Failed to call lib function", ret);
        return -1;
    }

    // verify_peer_enclave_trust thing
    // just verify initiator_identity
    
/*    session_info.session_id = session_id;
    session_info.status = 2;    // ACTIVE
    session_info.active.counter = 0;
    tmp = sizeof(sgx_key_128bit_t);
    for (int i = 0; i < tmp; i++) {
        (char*)(&session_info->active.AEK) + i = (char*)&dh_aek + i;
    }
    tmp = sizeof(sgx_key_128bit_t);
    for (int i = 0; i < tmp; i++) {
        (char*)&dh_aek + i = 0;
    }*/

    ocall_prints("Receiver AEK: \n");
    for (int i = 0; i < tmp; i++) {
        ocall_printx(*((char*)aek + i));
        ocall_prints(" ");
    }
    ocall_prints("\n");

    return 0;
}
