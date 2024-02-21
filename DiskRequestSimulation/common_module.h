#ifndef COMMON_MODULE_H
#define COMMON_MODULE_H
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH 1000
#define MAX_WORD_LENGTH 100

#define MAX_NO_OPERATIONS 100

#define GROUP_ID 93118274

#define USER_KERNEL_UPQUEUE_ID 543
#define USER_KERNEL_DOWNQUEUE_ID 345
#define DISK_KERNEL_UPQUEUE_ID 123
#define DISK_KERNEL_DOWNQUEUE_ID 321

struct IORequest{
    long mtype;
    char mtext[64];
    int PID;
};

int createQueue(int keyID){
    int msgqid= msgget(keyID, IPC_CREAT | 0644);
    if (msgqid == -1){
        perror("error in create");
        exit(-1);
    }
    return msgqid;
}


char* itoa(int value) {
    int num_digits = snprintf(NULL, 0, "%d", value);
    char* str = (char*)malloc(num_digits + 1);
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    sprintf(str, "%d", value);
    return str;
}

int atoi(const char* str) {
    int result;
    sscanf(str, "%d", &result);
    return result;
}
#endif // COMMON_MODULE_H

