#include "const.h"

int createSharedMemory() {
    key_t key = ftok(getenv("HOME"), 'S');
    if (key < 0) {
        printf("Error with key,SharedMemory,%d\n", key);
        exit(1);
    }
    key += 1;
    int sharedMemoryId = semget(key, 6, 0777 | IPC_CREAT);
    if (sharedMemoryId < 0) {
        printf("Error with shared memory\n");
        exit(1);
    }
    union semun basic;
    union semun put_o;
    union semun put_t;
    union semun take;
    basic.val = 1;
    put_o.val = OVEN_SIZE;
    put_t.val = TABLE_SIZE;
    take.val = 0;
    if (semctl(sharedMemoryId, IN_OVEN, SETVAL, basic) < 0 ||
        semctl(sharedMemoryId, TAKE_OUT_OVEN, SETVAL, take) < 0 ||
        semctl(sharedMemoryId, PUT_IN_OVEN, SETVAL, put_o) < 0 ||
        semctl(sharedMemoryId, ON_TABLE, SETVAL, basic) < 0 ||
        semctl(sharedMemoryId, TAKE_OUT_TABLE, SETVAL, take) < 0 ||
        semctl(sharedMemoryId, PUT_ON_TABLE, SETVAL, put_t) < 0
        ){
        printf("Error with SEMAPHORES values\n");
        exit(1);
    }
    return sharedMemoryId;
}

int createOven() {
    key_t key = ftok("./chef", 'O');
    if (key < 0) {
        printf("Error with create Oven key,%d\n",key);
        exit(1);
    }
    int ovenId = shmget(key, sizeof(struct Oven), IPC_CREAT | 0777);
    if (ovenId < 0) {
        printf("Error with create Oven Id\n");
        exit(1);
    }
    int temp = ovenId;
    struct Oven *oven = shmat(ovenId, NULL, 0);
    oven->pizzasCount = 0;
    oven->pizzaGoingOutside = 0;
    oven->pizzaGoingInside = 0;
    for (int i = 0; i < OVEN_SIZE; i++) {
        oven->tab[i] = -1;
    }
    return temp;
}

int createTable() {
    key_t key = ftok("./supplier", 'T');
    if (key < 0) {
        printf("Error with create Table key\n");
        exit(1);
    }
    int tableId = shmget(key, sizeof(struct Table), IPC_CREAT | 0777);
    if (tableId < 0) {
        printf("Error with create Table Id\n");
        exit(1);
    }
    int temp = tableId;
    struct Oven *table = shmat(tableId, NULL, 0);
    table->pizzasCount = 0;
    table->pizzaGoingOutside = 0;
    table->pizzaGoingInside = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->tab[i] = -1;
    }
    return temp;
}

int sharedMemoryId;
int ovenId;
int tableId;

void handler(int signum) {
    semctl(sharedMemoryId, 0, IPC_RMID, NULL);
    shmctl(ovenId, IPC_RMID, NULL);
    shmctl(tableId, IPC_RMID, NULL);
}


int main(int argc, char **argv) {
    if (argc != 3) {
        puts("missing arguments\n");
        return 1;
    }

    signal(SIGINT, handler);

    sharedMemoryId = createSharedMemory();
    ovenId = createOven();
    tableId = createTable();

    for (int i = 0; i < atoi(argv[1]); i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./cook", "./cook", NULL);
        }
    }

    for (int i = 0; i < atoi(argv[2]); i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./deliverer", "./deliverer", NULL);
            printf("Return not expected, execl() error\n");

        }
    }

    for (int i = 0; i < atoi(argv[2]) + atoi(argv[1]); i++)
        wait(NULL);


    return 0;
}