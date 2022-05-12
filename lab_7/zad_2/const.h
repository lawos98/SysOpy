#ifndef CONST
#define CONST

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>

#define OVEN_SHM "shm_oven"
#define TABLE_SHM "shm_table"

#define IN_OVEN "/IN_OVEN_SEMAPHORE"
#define TAKE_OUT_OVEN "/TAKE_OUT_OVEN_SEMAPHORE"
#define PUT_IN_OVEN "/PUT_IN_OVEN_SEMAPHORE"
#define ON_TABLE "/ON_TABLE_SEMAPHORE"
#define TAKE_OUT_TABLE "/TAKE_OUT_TABLE_SEMAPHORE"
#define PUT_ON_TABLE "/PUT_ON_TABLE_SEMAPHORE"

#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define MAKE_TIME 4
#define BAKE_TIME 4
#define DELIVERY_TIME 4
#define RETURN_TIME 4

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

void lock(sem_t* sem){
    if (sem_wait(sem) == -1){
        printf("Locking semaphore\n");
        exit(1);
    }
}

void unlock(sem_t* sem) {
    if (sem_post(sem) == -1){
        printf("Unlocking semaphore\n");
        exit(1);
    }
}

char* getTime(){
    time_t timeNow=time(NULL);
    return asctime(gmtime(&timeNow));
}

sem_t* getSharedMemoryID(char* name){
    sem_t* sem=sem_open(name, O_RDWR);
    if(sem == SEM_FAILED){
        printf("Error with sem %s!\n", name);
        exit(1);
    }
    return sem;
}

int getOvenID(){
    int shm_oven_fd=shm_open(OVEN_SHM, O_RDWR , 0666);
    if (shm_oven_fd < 0){
        printf("Error with Oven ID!\n");
        exit(1);
    }
    return shm_oven_fd;
}


int getTableID(){
    int shm_table_fd=shm_open(TABLE_SHM, O_RDWR , 0666);
    if (shm_table_fd<0){
        printf("Error with Table ID!\n");
        exit(1);
    }
    return shm_table_fd;
}


#endif