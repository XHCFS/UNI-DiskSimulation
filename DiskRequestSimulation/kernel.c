#include "kernel.h"

void logEvent(int PID, const char* message,int messageType) {
    const char* action;

    if (messageType == 1) {
        action = "request to add";
    }

    else if (messageType == 5) {
        action = "request to delete";
    } 

    else if (messageType == 2) {
        printf("At time = %d, %s %s empty slots \n", CLK, "Disk status =", message);
        return;
    } 

    else if (messageType == 3) {
        
        action = (message[0] == '0' || message[0] == '1') ? "send success to" : "send failure to";
        printf("At time = %d, %s Process%d\n", CLK, action, PID);
        return;
    } 

    else {
        action = "unknown action";
    }
    printf("At time = %d, %s %s from P%d\n", CLK, action, message, PID);

}

int CLKSignal(){
    CLK++;
    for (int i =0;i<numProcesses;i++){
        if (kill(userPIDs[i], SIGUSR2)!=0) {perror("Error sending signal");}
    }

    if (kill(diskPID, SIGUSR2) != 0) {perror("Error sending signal");}

    sleep(1);    
    return 0;

}


void handle_sigint(int signal) {
    int array[] = {userUplinkID, userDownlinkID, diskUplinkID, diskDownlinkID};

    for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
        if (msgctl(array[i], IPC_RMID, NULL) == 0) {
            printf("Message queue %d removed successfully.\n", array[i]);
        } else {
            perror("msgctl");
        }
    }
    exit(EXIT_SUCCESS);
}

int main(){
        
        // INITILIZATIONS
        signal(SIGINT, handle_sigint);
        userUplinkID=createQueue(USER_KERNEL_UPQUEUE_ID);
        userDownlinkID=createQueue(USER_KERNEL_DOWNQUEUE_ID);
        diskUplinkID=createQueue(DISK_KERNEL_UPQUEUE_ID);
        diskDownlinkID=createQueue(DISK_KERNEL_DOWNQUEUE_ID);
        struct IORequest response, request;
        // RECEIVING PROCESSID
        msgrcv(diskUplinkID,&response, sizeof(struct IORequest),4,0);
        diskPID=response.PID;
        
        char output[100];
        printf("Enter the number of processes: ");
        scanf("%99s", output);
        numProcesses=atoi(output);

        for (int i=0;i<numProcesses;i++){
                msgrcv(userUplinkID,&response, sizeof(struct IORequest),4,0);
                userPIDs[i]=response.PID;
        }

        while(1){
                CLKSignal();
                
                if (hold==1){
                        ret = msgrcv(diskUplinkID,&response, sizeof(struct IORequest),0, IPC_NOWAIT);
                        if (ret==-1){continue;}
                        msgsnd(userDownlinkID,&response, sizeof(struct IORequest),0);   
                        logEvent(response.PID, response.mtext , response.mtype);
                        hold=0;
                }
                if (hold ==0){
                        ret = msgrcv(userUplinkID, &request, sizeof(struct IORequest), 0, IPC_NOWAIT);
                        if (ret ==-1){continue;}
                        logEvent(request.PID,  request.mtext , request.mtype);
                        if (request.mtype==1){
                                if (kill(diskPID, SIGUSR1) != 0) {perror("Error sending signal 2");}
                                printf("at time = %d, sent status request to Disk\n",CLK);
                                msgrcv(diskUplinkID,&response,sizeof(struct IORequest),0,0);
                                logEvent(response.PID, response.mtext , response.mtype);
                                int availableSlots=atoi(response.mtext);
                                if (availableSlots>0){
                                        msgsnd(diskDownlinkID,&request, sizeof(struct IORequest),0);
                                        hold=1;
                                 }
                                 else{
                                        response.mtype=3;
                                        strcpy(response.mtext,"2");
                                        msgsnd(userDownlinkID,&response, sizeof(struct IORequest),0);   
                                        logEvent(response.PID, response.mtext , response.mtype);
                                 }
                        }
                        else if (request.mtype==5){
                                msgsnd(diskDownlinkID,&request,sizeof(struct IORequest),0);
                                hold=1;
                        }
                }

        }
}
