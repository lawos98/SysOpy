#include "const.h"


void putInOven(struct Oven* oven,int type){
    oven->tab[oven->pizzaGoingInside]=type;
    oven->pizzasCount+=1;
    oven->pizzaGoingInside=(oven->pizzaGoingInside+1)%OVEN_SIZE;
}
int takeOutOven(struct Oven* oven){
    int type=oven->tab[oven->pizzaGoingOutside];
    oven->tab[oven->pizzaGoingOutside]=-1;
    oven->pizzasCount-=1;
    oven->pizzaGoingOutside=(oven->pizzaGoingOutside+1)%OVEN_SIZE;
    return type;
}
void putOnTable(struct Table* table,int type){
    table->tab[table->pizzaGoingOn]=type;
    table->pizzasCount+=1;
    table->pizzaGoingOn=(table->pizzaGoingOn+1)%TABLE_SIZE;
}

int main() {
    srand(getpid());

    sem_t* inOvenSem = getSharedMemoryID(IN_OVEN);
    sem_t* putInOvenSem = getSharedMemoryID(PUT_IN_OVEN);
    sem_t* takeOutOvenSem = getSharedMemoryID(TAKE_OUT_OVEN);
    sem_t* onTableSem = getSharedMemoryID(ON_TABLE);
    sem_t* putOnTableSem = getSharedMemoryID(PUT_ON_TABLE);
    sem_t* takeOutTableSem = getSharedMemoryID(TAKE_OUT_TABLE);

    int ovenId=getOvenID();
    int tableId=getTableID();

    struct Oven* oven= mmap(NULL, sizeof(struct Oven), PROT_READ | PROT_WRITE, MAP_SHARED, ovenId, 0);
    struct Table* table= mmap(NULL, sizeof(struct Table), PROT_READ | PROT_WRITE, MAP_SHARED, tableId, 0);

    while(1){
        int type=rand()%10;
        sleep(MAKE_TIME);
        printf("Time: %s Cook[pid:%d] made pizza %d\n\n",getTime(),getpid(),type);


        lock(putInOvenSem);
        lock(inOvenSem);
        putInOven(oven,type);
        printf("Time: %s Cook[pid:%d] put in oven pizza %d.Total number in Oven is %d\n\n",getTime(),getpid(),type,oven->pizzasCount);
        unlock(inOvenSem);
        unlock(takeOutOvenSem);

        sleep(BAKE_TIME);

        lock(takeOutOvenSem);
        lock(inOvenSem);
        type=takeOutOven(oven);
        printf("Time: %s Cook[pid:%d] take out oven pizza %d\n\n",getTime(),getpid(),type);
        unlock(inOvenSem);
        unlock(putInOvenSem);

        lock(putOnTableSem);
        lock(onTableSem);
        putOnTable(table,type);
        printf("Time: %s Cook[pid:%d] put on table pizza %d.Total number on Table is %d\n\n",getTime(),getpid(),type,table->pizzasCount);
        unlock(onTableSem);
        unlock(takeOutTableSem);
    }
}