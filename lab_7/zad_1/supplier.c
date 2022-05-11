#include "const.h"

int takeOutTable(struct Table* table){
    int type=table->tab[table->pizzaGoingOutside];
    table->tab[table->pizzaGoingOutside]=-1;
    table->pizzaGoingOutside=(table->pizzaGoingOutside+1)%TABLE_SIZE;
    table->pizzasCount-=1;
    return type;
}
int main(){
    srand(time(NULL));

    int sharedMemoryId=getSharedMemoryID();
    int tableId=getTableID();

    struct Table* table=shmat(tableId,NULL,0);

    while(1){
        lock(sharedMemoryId,TAKE_OUT_TABLE);
        lock(sharedMemoryId,ON_TABLE);
        int type=takeOutTable(table);
        unlock(sharedMemoryId,PUT_ON_TABLE);
        printf("Supplier[pid:%d] take out pizza %d from table at %s \n",getpid(),type,getTime());
        unlock(sharedMemoryId,ON_TABLE);

        sleep(DELIVERY_TIME);
        printf("Supplier[pid:%d] delivery pizza %d at %s \n",getpid(),type,getTime());

        sleep(RETURN_TIME);
        printf("Supplier[pid:%d] return after delivery pizza %d at %s \n",getpid(),type,getTime());

    }
}