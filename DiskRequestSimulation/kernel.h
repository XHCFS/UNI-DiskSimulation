#include "common_module.h"



void logEvent(int PID, const char* message,int messageType) ;
int CLKSignal(); 
int hold=0;
int numProcesses;
int ret;
int userUplinkID,userDownlinkID,diskUplinkID,diskDownlinkID;
int CLK=0;
int diskPID;
int userPIDs[3];
int main();
