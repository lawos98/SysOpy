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
int main(){
    srand(time(NULL));

    int sharedMemoryId=getSharedMemoryID();
    int ovenId=getOvenID();
    int tableId=getTableID();

    struct Oven* oven= shmat(ovenId,NULL, 0);
    struct Table* table=shmat(tableId, NULL, 0);

    while(1){
        int type=rand()%10;
        printf("Cook[pid:%d] make pizza %d at %s \n",getpid(),type,getTime());
        sleep(MAKE_TIME);

        lock(sharedMemoryId,PUT_IN_OVEN);
        lock(sharedMemoryId,IN_OVEN);
        putInOven(oven,type);
        printf("Cook[pid:%d] put in oven pizza %d at %s \n",getpid(),type,getTime());
        unlock(sharedMemoryId,IN_OVEN);
        unlock(sharedMemoryId,TAKE_OUT_OVEN);

        sleep(BAKE_TIME);

        lock(sharedMemoryId,TAKE_OUT_OVEN);
        lock(sharedMemoryId,IN_OVEN);
        type=takeOutOven(oven);
        printf("Cook[pid:%d] take out oven pizza %d at %s \n",getpid(),type,getTime());
        unlock(sharedMemoryId,IN_OVEN);
        unlock(sharedMemoryId,PUT_IN_OVEN);

        lock(sharedMemoryId,PUT_ON_TABLE);
        lock(sharedMemoryId,ON_TABLE);
        putOnTable(table,type);
        printf("Cook[pid:%d] put on table pizza %d at %s \n",getpid(),type,getTime());
        unlock(sharedMemoryId,ON_TABLE);
        unlock(sharedMemoryId,TAKE_OUT_TABLE);
    }
}