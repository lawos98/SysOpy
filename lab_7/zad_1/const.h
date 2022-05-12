#ifndef CONST
#define CONST

#include <time.h>
#include <sys/ipc.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

//SEMAPHORES
#define IN_OVEN 0
#define TAKE_OUT_OVEN 1
#define PUT_IN_OVEN 2
#define ON_TABLE 3
#define TAKE_OUT_TABLE 4
#define PUT_ON_TABLE 5

#define MAKE_TIME 4
#define BAKE_TIME 4
#define DELIVERY_TIME 4
#define RETURN_TIME 4

#define OVEN_SIZE 5
#define TABLE_SIZE 5

struct Oven{
    int tab[OVEN_SIZE];
    int pizzasCount;
    int pizzaGoingInside;
    int pizzaGoingOutside;
};
struct Table{
    int tab[TABLE_SIZE];
    int pizzasCount;
    int pizzaGoingOn;
    int pizzaGoingOutside;
};

union semun {
    int val;
    struct semid_ds *buf;
    short *array;
};

int getOvenID(){
    key_t key_o=ftok("./chef",'O');
    int sem_id=shmget(key_o,sizeof(struct Oven),0);
    if(sem_id<0){
        printf("Error with get Oven ID");
        exit(1);
    }
    return sem_id;
}
int getTableID(){
    key_t key_o=ftok("./supplier",'T');
    int sem_id=shmget(key_o,sizeof(struct Table),0);
    if(sem_id<0){
        printf("Error with get Table ID");
        exit(1);
    }
    return sem_id;
}

int getSharedMemoryID(){
    key_t key_o=ftok(getenv("HOME"), 'S')+1;
    int sem_id=semget(key_o,6,0);
    if(sem_id<0){
        printf("Error with get Shared Memory ID");
        exit(1);
    }
    return sem_id;
}

char* getTime(){
    time_t timeNow=time(NULL);
    return asctime(gmtime(&timeNow));
}

void lock(int id,int number){
    struct sembuf s;
    s.sem_num=number;
    s.sem_op=-1;
    if(semop(id,&s,1)<0){
        printf("Error with lock");
        exit(1);
    }
}

void unlock(int id,int number){
    struct sembuf s;
    s.sem_num=number;
    s.sem_op=1;
    if(semop(id,&s,1)<0){
        printf("Error with lock");
        exit(1);
    }
}

#endif