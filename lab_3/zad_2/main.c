#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

clock_t startTime,endTime;
struct tms timerStartTms,timerEndTms;
void startTimer(){
    startTime = times(&timerStartTms);
}
void endTimer(){
    endTime=times(&timerEndTms);
}
void printToFile(){
    double realTime=(double)(endTime-startTime)/sysconf(_SC_CLK_TCK);
    double userTime=(double)(timerEndTms.tms_cutime-timerStartTms.tms_cutime)/sysconf(_SC_CLK_TCK);
    double sysTime=(double)(timerEndTms.tms_cstime-timerStartTms.tms_cstime)/sysconf(_SC_CLK_TCK);
    printf("realTime: %f;userTime: %f;sysTime: %f \n",realTime,userTime,sysTime);

}

int main(int argc,char** argv) {
    int n=atoi(argv[2]);
    double lengthOfBlock=atof(argv[1]);
    if(n*lengthOfBlock>1){
        printf("Incorrect number of blocks or length of blocks");
        exit(301);
    }
    else if(argc<1){
        printf("Incalid number of arguments");
        exit(302);
    }
    startTimer();
    calculateFunction(n,lengthOfBlock);
    endTimer();
    printToFile();
    return 0;
}
