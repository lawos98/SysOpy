#include "const.h"

int takeOutTable(struct Table* table){
    int type=table->tab[table->pizzaGoingOutside];
    table->tab[table->pizzaGoingOutside]=-1;
    table->pizzaGoingOutside=(table->pizzaGoingOutside+1)%TABLE_SIZE;
    table->pizzasCount-=1;
    return type;
}
int main(){
    srand(getpid());

    int sharedMemoryId=getSharedMemoryID();
    int tableId=getTableID();

    struct Table* table=shmat(tableId,NULL,0);

    while(1){
        lock(sharedMemoryId,TAKE_OUT_TABLE);
        lock(sharedMemoryId,ON_TABLE);
        int type=takeOutTable(table);
        printf("Time: %s Supplier[pid:%d] take out pizza %d from table\n\n",getTime(),getpid(),type);
        unlock(sharedMemoryId,PUT_ON_TABLE);
        unlock(sharedMemoryId,ON_TABLE);

        sleep(DELIVERY_TIME);
        printf("Time: %s Supplier[pid:%d] delivery pizza %d\n\n",getTime(),getpid(),type);

        sleep(RETURN_TIME);
        printf("Time: %s Supplier[pid:%d] return after delivery pizza %d\n\n",getTime(),getpid(),type);

    }
}