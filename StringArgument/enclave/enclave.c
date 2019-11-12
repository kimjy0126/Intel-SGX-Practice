#include "enclave_t.h"

void ecall_reverse_pw(char* pw)
{
    ocall_print("Start of ecall_reverse_pw");

    int len = 0;
    while (pw[len]) {
        len++;
    }

    for (int i = 0; i < len / 2; i++) {
        char tmp = pw[i];
        pw[i] = pw[len - i - 1];
        pw[len - i - 1] = tmp;
    }

    ecall_process_pw(pw);

    ocall_print("End of ecall_reverse_pw");
}

void ecall_process_pw(char* pw)
{
    ocall_print("Start of ecall_process_pw");

    int len = 0;
    while (pw[len]) {
        len++;
    }

    for (int i = 3; i < len; i++) {
        pw[i] = '*';
    }

    ocall_print("End of ecall_process_pw");
}
