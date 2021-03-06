#include "const.h"

int sharedMemoryId;
int ovenId;
int tableId;

int createSharedMemory() {
    key_t key = ftok(getenv("HOME"), 'S');
    if (key < 0) {
        printf("Error with key,SharedMemory,%d\n", key);
        exit(1);
    }
    key += 1;
    int sharedMemoryId = semget(key, 6, 0777 | IPC_CREAT);
    printf("%d",semctl(sharedMemoryId, 0, GETVAL, NULL));

    if (sharedMemoryId < 0) {
        printf("Error with shared memory\n");
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
    struct Table *table = shmat(tableId, NULL, 0);
    table->pizzasCount = 0;
    table->pizzaGoingOutside = 0;
    table->pizzaGoingOn = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->tab[i] = -1;
    }
    return temp;
}

int sharedMemoryId;
int ovenId;
int tableId;

void handler(int signum){
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
            execl("./chef", "./chef", NULL);
        }
    }

    for (int i = 0; i < atoi(argv[2]); i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./supplier", "./supplier", NULL);
        }
    }

    for (int i = 0; i < atoi(argv[2]) + atoi(argv[1]); i++)
        wait(NULL);

    return 0;
}