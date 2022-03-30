#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void createProcess(int n){
    if(n>0){
        pid_t child_pid;
        child_pid = vfork();
        if (child_pid != 0) {
            createProcess(n-1);
        } else {
            printf("PID : %d \n",(int)getpid());
            printf("Child proccess :Parent number :%d  \n", (int)getppid());
        }
    }
    exit(0);
}