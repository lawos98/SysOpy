#include "const.h"

int takeOutTable(struct Table* table){
    int type=table->tab[table->pizzaGoingOutside];
    table->tab[table->pizzaGoingOutside]=-1;
    table->pizzaGoingOutside=(table->pizzaGoingOutside+1)%TABLE_SIZE;
    table->pizzasCount-=1;
    return type;
}

int main() {
    srand(getpid());

    sem_t* onTableSem =getSharedMemoryID(ON_TABLE);
    sem_t* putOnTableSem = getSharedMemoryID(PUT_ON_TABLE);
    sem_t* takeOutTableSem = getSharedMemoryID(TAKE_OUT_TABLE);

    int tableID = getTableID();

    struct Table* table = mmap(NULL, sizeof(struct Table), PROT_READ | PROT_WRITE, MAP_SHARED, tableID, 0);

    while (1) {
        lock(takeOutTableSem);
        lock(onTableSem);
        int type=takeOutTable(table);
        printf("Time: %s Supplier[pid:%d] take out pizza %d from table\n\n",getTime(),getpid(),type);
        unlock(putOnTableSem);
        unlock(onTableSem);

        sleep(DELIVERY_TIME);
        printf("Time: %s Supplier[pid:%d] delivery pizza %d\n\n",getTime(),getpid(),type);

        sleep(RETURN_TIME);
        printf("Time: %s Supplier[pid:%d] return after delivery pizza %d\n\n",getTime(),getpid(),type);
    }
}