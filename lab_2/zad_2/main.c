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
    char searchChar;
    if(argc>3){
        printf("To much arguments");
        return 1;
    }
    else if(argc==3){
        fileIn=argv[1];
        searchChar=argv[2][0];
    }
    else if(argc==2){
        fileIn=argv[1];
        printf("Please write file to Input \n");
        scanf("%s",fileIn);
    }
    else if(argc==1){
        printf("Please write char to find \n");
        scanf("%c",&searchChar);
        printf("Please write file to Input \n");
        scanf("%s",fileIn);
    }
    printf("|LIB|");
    startTimer();
    countFileLib(fileIn,searchChar);
    endTimer();
    printToFile();
    printf("|SYS|");
    startTimer();
    countFileSys(fileIn,searchChar);
    endTimer();
    printToFile();
    return 0;
}
