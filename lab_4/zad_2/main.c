
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void handler(int sig,siginfo_t *sigInfo,void *context){
    printf("Signal number: %d\n",sigInfo->si_signo);
    printf("User time: %ld \n",sigInfo->si_utime);
    printf("Sys time: %ld \n",sigInfo->si_stime);
    printf("Send process ID: %d \n",sigInfo->si_pid);
    printf("Real user ID: %d \n",sigInfo->si_uid);
    printf("Error value: %d \n",sigInfo->si_errno);
}
void testSIGINFO(){
    printf("test SIGINFO \n");
    struct sigaction act;
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=handler;
    sigemptyset(&act.sa_mask);
    if(sigaction(SIGUSR1,&act,NULL)==-1 || sigaction(SIGUSR2,&act,NULL)==-1 || sigaction(SIGRTMIN,&act,NULL)==-1){
        printf("Signal not be handle \n");
        exit(1);
    }
    raise(SIGUSR1);
    raise(SIGUSR2);
    raise(SIGRTMIN);
}

void testRESETHAND(){
    printf("test RESETHAND\n");
    struct sigaction act;
    act.sa_flags=SA_RESETHAND;
    act.sa_sigaction=handler;
    sigemptyset(&act.sa_mask);
    if(sigaction(SIGUSR1,&act,NULL)==-1){
        printf("Signal not be handle \n");
        exit(1);
    }
    raise(SIGUSR1);
}
void testNOCLDSTOP(){
    printf("test NOCLDSTOP\n");
    struct sigaction act;
    act.sa_flags=SA_NOCLDSTOP;
    act.sa_sigaction=handler;
    sigemptyset(&act.sa_mask);
    if(sigaction(SIGCHLD,&act,NULL)==-1){
        printf("Signal not be handle \n");
        exit(1);
    }
    pid_t pid=fork();
    if(pid==0){
        raise(SIGSTOP);
    }
    sleep(10);
}

int main(int argc,char** argv) {
    testSIGINFO();
    testNOCLDSTOP();
    testRESETHAND();
    return 0;
}
