
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>


int counterRecived=0;
int counter=0;
char* currentMode="";
int mode=0;

void handler1(int sig,siginfo_t *sigInfo,void *context){
    printf("Handler 1 \n");
    counterRecived++;
}
void handler2(int sig,siginfo_t *sigInfo,void *context){
    printf("Handler 2 \n");
    printf("Receive : %d \n",counterRecived);
    pid_t PID = sigInfo->si_pid;
    counter=0;
    while(counter<counterRecived){
        if(strcmp(currentMode,"queue")==0){
            union sigval v;
            v.sival_int=counter;
            sigqueue(PID,SIGUSR1,v);
        }
        else{
            if(strcmp(currentMode,"kill"))mode=SIGUSR1;
            else mode=SIGRTMIN+1;
            kill(PID,mode);
        }
        counter++;
    }
    if(strcmp(currentMode,"queue")==0){
        union sigval v;
        sigqueue(PID,SIGUSR2,v);
    }
    else{
        if(strcmp(currentMode,"kill"))mode=SIGUSR2;
        else mode=SIGRTMIN+2;
        kill(PID,mode);
    }
    exit(0);
}

int main(int argc,char** argv) {
    if(argc<2){
        printf("missing arguments\n");
        exit(1);
    }

    strcpy(currentMode,argv[1]);

    struct sigaction sig1;
    sigemptyset(&sig1.sa_mask);
    sig1.sa_sigaction = handler1;

    struct sigaction sig2;
    sigemptyset(&sig2.sa_mask);
    sig2.sa_flags =SA_SIGINFO;
    sig2.sa_sigaction =handler2;

    if(strcmp(currentMode,"sigrt")==0){
        sigaction(SIGRTMIN+1,&sig1,NULL);
        sigaction(SIGRTMIN+2,&sig2,NULL);
    }
    else{
        sigaction(SIGUSR1,&sig1,NULL);
        sigaction(SIGUSR2,&sig2,NULL);
    }

    sigset_t mask;
    sigfillset(&mask);
    if(strcmp(currentMode,"sigrt")==0){
        sigdelset(&mask,SIGRTMIN+1);
        sigdelset(&mask,SIGRTMIN+2);
    }
    else{
        sigdelset(&mask,SIGUSR1);
        sigdelset(&mask,SIGUSR2);
    }
    sigprocmask(SIG_SETMASK, &mask,NULL);

    while(1){
        sigsuspend(&mask);
    }
    return 0;
}
