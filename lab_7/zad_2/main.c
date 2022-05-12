#include "const.h"

sem_t* create_semaphore(char* name, int value){
    sem_t* new_sem = sem_open(name, O_CREAT, 0666, value);
    if (new_sem == SEM_FAILED){
        printf("Error with creating semaphore %s\n", name);
        exit(1);
    }
    return new_sem;
}

void create_semaphores() {
    sem_t* inOvenSem = create_semaphore(IN_OVEN, 1);
    sem_t* putInOvenSem = create_semaphore(PUT_IN_OVEN, OVEN_SIZE);
    sem_t* takeOutOvenSem = create_semaphore(TAKE_OUT_OVEN, 0);
    sem_t* onTableSem = create_semaphore(ON_TABLE, 1);
    sem_t* putOnTableSem = create_semaphore(PUT_ON_TABLE, TABLE_SIZE);
    sem_t* takeOutTableSem = create_semaphore(TAKE_OUT_TABLE, 0);
}

void createOven() {
    int shm_oven = shm_open(OVEN_SHM, O_RDWR | O_CREAT, 0666);
    if (shm_oven < 0) {
        printf("Error with create Oven key,%d\n",shm_oven);
        exit(1);
    }
    struct Oven *oven = mmap(NULL, sizeof(struct Oven), PROT_READ | PROT_WRITE, MAP_SHARED, shm_oven, 0);
    oven->pizzasCount = 0;
    oven->pizzaGoingOutside = 0;
    oven->pizzaGoingInside = 0;
    for (int i = 0; i < OVEN_SIZE; i++) {
        oven->tab[i] = -1;
    }
}

void createTable() {
    int shm_table= shm_open(TABLE_SHM, O_RDWR | O_CREAT, 0666);
    if (shm_table < 0) {
        printf("Error with create Table key\n");
        exit(1);
    }

    struct Table *table = mmap(NULL, sizeof(struct Table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_table, 0);
    table->pizzasCount = 0;
    table->pizzaGoingOutside = 0;
    table->pizzaGoingOn = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->tab[i] = -1;
    }
}
void handler(int signum){
    sem_unlink(IN_OVEN);
    sem_unlink(TAKE_OUT_OVEN);
    sem_unlink(PUT_IN_OVEN);
    sem_unlink(ON_TABLE);
    sem_unlink(PUT_ON_TABLE);
    sem_unlink(TAKE_OUT_TABLE);
}

int main(int argc, char **argv) {
    if(argc != 3) {
        puts("WRONG NUMBER OF ARGUMENTS\n");
        return 1;
    }

    signal(SIGINT, handler);

    create_semaphores();
    createOven();
    createTable();

    for (int i = 0; i < atoi(argv[1]); i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./chef", "./chef", NULL);
        }
    }

    for (int i = 0; i < atoi(argv[2]); i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./supplier", "./supplier", NULL);
        }
    }

    for(int i = 0; i < atoi(argv[2]) + atoi(argv[1]); i++)
        wait(NULL);


    return 0;
}