#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include "lib.h"

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
    printf("realTime: %f;userTime: %f;sysTime: %f\n",realTime,userTime,sysTime);

}

int main(int argc,char** argv) {
    char* fileIn=calloc(1024,sizeof(char));
    char* fileOut=calloc(1024,sizeof(char));
    if(argc>3){
        printf("To much arguments");
        return 1;
    }
    else if(argc==3){
        fileIn=argv[1];
        fileOut=argv[2];
    }
    else if(argc==2){
        fileIn=argv[1];
        printf("Please write file to Output \n");
        scanf("%s",fileOut);
    }
    else if(argc==1){
        printf("Please write file to Input \n");
        scanf("%s",fileIn);
        printf("Please write file to Output \n");
        scanf("%s",fileOut);
    }
    printf("|LIB|");
    startTimer();
    convertFileLib(fileIn,fileOut);
    endTimer();
    printToFile();
    printf("|SYS|");
    startTimer();
    convertFileSys(fileIn,fileOut);
    endTimer();
    printToFile();
    return 0;
}
