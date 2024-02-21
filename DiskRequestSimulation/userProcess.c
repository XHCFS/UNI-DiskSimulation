#include "userProcess.h"


void CLKsignalHandler(int sig_num){
    CLK++;
    signal(SIGUSR2, CLKsignalHandler);
}

void readRequests(char* in){
    FILE* file = fopen(in,"r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    int i=0;
    while(fgets(line, MAX_LINE_LENGTH,file)){
        line[strcspn(line, "\n")] = '\0';

        struct IOInfo new;

        char *word = strtok(line, " ");

        new.time=atoi(word);
        word=strtok(NULL, " ");
        
        if (strcmp(word,"ADD")==0) {
            new.Operation=1;
            word=strtok(NULL, "\"");
        }
        else if (strcmp(word,"DELETE")==0) {
            new.Operation=5;
            word=strtok(NULL, " ");
 
        }

        strcpy(new.input,word);
        Requests[i]=new;i++;
        
    }
    fclose(file);


}


int main(){
// INITILIZATIONS
    signal(SIGUSR2, CLKsignalHandler);
    PID=getpid();
    printf("Enter the file name to read from: ");
    scanf("%99s", filename);
    readRequests(filename);
    int upLinkID,downLinkID;
    struct IORequest response, request;
    upLinkID=createQueue(USER_KERNEL_UPQUEUE_ID);
    downLinkID=createQueue(USER_KERNEL_DOWNQUEUE_ID);

// SENDING PID
    request.mtype=4;
    request.PID=PID;
    strcpy(request.mtext,"PID sent");
    msgsnd(upLinkID,&request,sizeof(struct IORequest),0);
    

    int size = sizeof(Requests) / sizeof(Requests[0]);
    int i=0;
// LOOPING THROUGH REQUESTS
    while( i < size){
       pause();
// WAIT UNTIL CLOCK SIGNAL IS RECEIVED
        if (Requests[i].time==CLK){
            strcpy(request.mtext,Requests[i].input);
            request.mtype=Requests[i].Operation;
            request.PID=PID;
            msgsnd(upLinkID,&request,sizeof(struct IORequest),0);
            msgrcv(downLinkID,&response,sizeof(struct IORequest),0,0);
            i++;
        }
    }

}
