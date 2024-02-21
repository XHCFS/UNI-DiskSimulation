#include "disk.h"


int getDiskInfo() {
    int i = 0;
    int availableSlots = 0;
    while (i < 10) {
        if (storage[i][0] == '\0' || strlen(storage[i]) == 0) {
            availableSlots++;
        }
        i++;
    }

    return availableSlots;
}

void CLKsignalHandler(int sig_num){
    CLK++;
    if (waitTime>=0) {
    waitTime--;
    }
    signal(SIGUSR2, CLKsignalHandler);
}


int add(char* data) {
    int i = 0; 
    while (i < slotNum) {
        if (storage[i][0] == '\0') {
            strcpy(storage[i], data);
            return 0;
        }
        i++;
    }
    return -1;
}


int delete(int slot){
    strcpy(storage[slot],"\0");
    return 1;
}

void statusRequestHandler(int sig_num){
    struct IORequest ioRequest;
    ioRequest.mtype=2;
    char* diskInfo=itoa(getDiskInfo());
    strcpy(ioRequest.mtext,diskInfo);
    msgsnd(upLinkID, &ioRequest, sizeof(ioRequest.mtext),0);
    signal(SIGUSR1, statusRequestHandler);
}

void waitCLK(int time){
    waitTime+=(time);
    while(waitTime>0){
        pause();
    }

}


int main(){

// INITILIZATIONS
    signal(SIGUSR2, CLKsignalHandler);
    signal(SIGUSR1, statusRequestHandler);
 
    PID=getpid();
    upLinkID=createQueue(DISK_KERNEL_UPQUEUE_ID);
    downLinkID=createQueue(DISK_KERNEL_DOWNQUEUE_ID);
    struct IORequest ioRequestDown, ioRequestUp, ioResponse;
        
// SENDING PROCESS ID TO KERNEL
    ioRequestUp.mtype=4;
    ioRequestUp.PID=PID;
    strcpy(ioRequestUp.mtext,"PID sent");

    msgsnd(upLinkID,&ioRequestUp,sizeof(struct IORequest),0);

    while(1){

        int rec=msgrcv(downLinkID,&ioRequestDown, sizeof(struct IORequest),0,0);
        printf("%d\n",rec);
        if (ioRequestDown.mtype==1 && rec!=-1){
            int out=add(ioRequestDown.mtext);
            waitCLK(3);
            char* result=itoa(out);
            strcpy(ioResponse.mtext,result);
            ioResponse.mtype=3;
            ioResponse.PID=ioRequestDown.PID;
            msgsnd(upLinkID,&ioResponse,sizeof(struct IORequest),0);

        }
        else if (ioRequestDown.mtype==5 && rec!=-1){
            char* requestText=ioRequestDown.mtext;
            int slot=atoi(requestText);
            int out=delete(slot);
            waitCLK(1);
            char* result=itoa(out);
            strcpy(ioResponse.mtext,result);
            ioResponse.mtype=3;
            ioResponse.PID=ioRequestDown.PID;
            msgsnd(upLinkID,&ioResponse, sizeof(struct IORequest),0);
        }
    }
    return 0;

}

