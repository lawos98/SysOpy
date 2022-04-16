
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>


int counterRecived=0;
int counterSignal=0;
int mode=0;

void handler1(int sig,siginfo_t *sigInfo,void *context){
    printf("Handler 1 \n");
    counterRecived++;
    if(sigInfo->si_code==SI_QUEUE){
        printf("value: %d\n",sigInfo->si_value.sival_int);
    }
}
void handler2(int sig,siginfo_t *sigInfo,void *context){
    printf("Handler 2 \n");
    printf("Receive : %d | Signals : %d\n",counterRecived,counterSignal);
    exit(0);
}

int main(int argc,char** argv) {
    if(argc<4){
        printf("missing arguments\n");
        exit(1);
    }
    int PID=atoi(argv[1]);
    printf("PID: %d \n",PID);
    counterSignal=atoi(argv[2]);

    struct sigaction sig1;
    sigemptyset(&sig1.sa_mask);
    sig1.sa_flags =SA_SIGINFO;
    sig1.sa_sigaction =handler1;

    struct sigaction sig2;
    sigemptyset(&sig2.sa_mask);
    sig2.sa_sigaction =handler2;

    if(strcmp(argv[3],"sigrt")==0){
        sigaction(SIGRTMIN+1,&sig1,NULL);
        sigaction(SIGRTMIN+2,&sig2,NULL);
    }
    else{
        sigaction(SIGUSR1,&sig1,NULL);
        sigaction(SIGUSR2,&sig2,NULL);
    }

    for(int i=0; i<counterSignal;i++){
        if(strcmp(argv[3],"queue")==0){
            union sigval v;
            sigqueue(PID,SIGUSR1,v);
        }
        else{
            if(strcmp(argv[3],"kill"))mode=SIGUSR1;
            else mode=SIGRTMIN+1;
            kill(PID,mode);
        }
    }
    printf("sent %d signals",counterSignal);

    if(strcmp(argv[3],"queue")==0){
        union sigval v;
        sigqueue(PID,SIGUSR2,v);
    }
    else{
        if(strcmp(argv[3],"kill"))mode=SIGUSR2;
        else mode=SIGRTMIN+2;
        kill(PID,mode);
    }

    while(1);
    return 0;
}
