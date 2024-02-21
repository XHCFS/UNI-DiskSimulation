#include "common_module.h"

int PID;
int CLK=0;
int slotNum=10;
volatile int waitTime=0;
int upLinkID;int downLinkID;
char storage[10][64];

void CLKsignalHandler(int sig_num);
void waitCLK(int waitTime);
int add(char* data);
int delete(int slot);
void statusRequestHandler(int sig_num);
int getDiskInfo();
int main();

