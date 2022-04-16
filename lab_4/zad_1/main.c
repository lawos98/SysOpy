
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

void handler(int sig){
    printf("Receive signal %d\n", sig);
}
void pending(int sig){
    sigset_t pendingSet;
    if(sigpending(&pendingSet)==-1){
        fprintf(stderr,"Pending signal not obtained %s\n", strerror(errno));
        exit(1);
    }
    if(sigismember(&pendingSet, sig)){
        printf("Signal is pending %d \n",sig);
    }
    else{
        printf("Signal is not pending %d \n",sig);
    }
}
void mask(int sig){
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, sig);
    if(sigprocmask(SIG_BLOCK,&mask,NULL)<0){
        fprintf(stderr,"Pending signal not obtained %s\n", strerror(errno));
        exit(1);
    }
}
void makeSignal(){
    raise(SIGUSR1);
    pid_t pid = fork();
    if(pid==0){
        raise(SIGUSR1);
    }
    else{
        wait(NULL);
    }
}


int main(int argc,char** argv) {
    if(argc < 1) {
        printf("missing arguments \n");
        exit(1);
    }
    if(strcmp(argv[1],"ignore")==0){
        signal(SIGUSR1,SIG_IGN);
        makeSignal();
    }
    else if(strcmp(argv[1],"handle")==0){
        signal(SIGUSR1,handler);
        makeSignal();
    }
    else if(strcmp(argv[1],"pending")==0){
        pending(SIGUSR1);
    }
    else if(strcmp(argv[1],"mask")==0){
        mask(SIGUSR1);
    }

    #ifdef FORK
    pid_t pid = fork();
    if(pid==0){
        if(strcmp(argv[1],"pending")==0){
            pending(SIGUSR1);
        }
        else{
            raise(SIGUSR1);
        }
        exit(0);
    }
    #endif

    #ifdef EXEC
    execl(argv[0],"exec_test",argv[1],NULL);
    #endif

    return 0;
}
