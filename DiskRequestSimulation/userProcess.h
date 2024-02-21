#include "common_module.h"

struct IOInfo {
    int time;
    int Operation;
    char input[64];
};

int CLK;
int PID;
struct IOInfo Requests[MAX_NO_OPERATIONS] ;
char filename[100];
void CLKsignalHandler(int sig_num);
void readRequests(char* file);

int main();
