#include "const.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>

void doStop(struct Message* msg,mqd_t desc,mqd_t serverDesc){
    msg->msgType=STOP;
    mq_send(serverDesc,(char*)&msg,sizeof(struct Message),0);
    mq_close(serverDesc);
    exit(0);
}

void doList(struct Message* msg,mqd_t desc,mqd_t serverDesc){
    msg->msgType=LIST;
    mq_send(serverDesc,(char*)&msg,sizeof(struct Message),0);
    mq_receive(desc,(char*)&msg,sizeof(struct Message),NULL);
    printf("msgType:\n%s \n",msg->msgText);
}
void doAll(struct Message* msg,mqd_t desc,mqd_t serverDesc){
    msg->msgType=ALL;
    mq_send(serverDesc,(char*)&msg,sizeof(struct Message),0);
}

void doOne(struct Message* msg,mqd_t desc,mqd_t serverDesc){
    msg->msgType=ONE;
    mq_send(serverDesc,(char*)&msg,sizeof(struct Message),0);
}

int doInit(struct Message* msg,mqd_t desc,mqd_t serverDesc){
    msg->msgType=INIT;
    mq_send(serverDesc,(char*)&msg,sizeof(struct Message),0);
    mq_receive(desc,(char*)&msg,sizeof(struct Message),NULL);
    printf("Init for user %d with queue key %s\n",msg->fromClient,msg->queueKey);
    return msg->fromClient;
}

int main(){
    srand(time(NULL));
    char queueName[8];
    queueName[0]='/';
    for(int i=1;i<8;i++){
        queueName[i]='A'+(rand()%26);
    }
    struct mq_attr attr;
    attr.mq_maxmsg=MAXUSERS;
    attr.mq_msgsize=sizeof(struct Message);
    mqd_t desc=mq_open(queueName,O_RDWR|O_CREAT,0777,&attr);
    mqd_t serverDesc=mq_open("/Server",O_RDWR);

    printf("queueName : %s\n",queueName);

    struct Message* msgInit=calloc(1,sizeof(struct Message));
    time_t t=time(NULL);
    msgInit->time=*localtime(&t);

    strncpy(msgInit->queueKey,queueName,8);

    int clientId= doInit(msgInit,desc,serverDesc);
    free(msgInit);

    char* command="";
    size_t length=0;
    ssize_t read;

    while(1){
        struct Message* msgRecived=calloc(1,sizeof(struct Message));
        while(mq_receive(desc,(char*)&msgRecived,sizeof(struct Message),NULL)>-1){
            printf("Message from %d \" %s \" \n",msgRecived->fromClient,msgRecived->msgText);
        }
        free(msgRecived);
        printf("Write command: \n");
        read= getline(&command,&length,stdin);
        command[read-1]='\0';
        if(strcmp(command,"")==0)continue;
        struct Message* msg=calloc(1,sizeof(struct Message));
        time_t t=time(NULL);
        msg->time=*localtime(&t);
        msg->fromClient=clientId;
        strncpy(msg->queueKey,queueName,8);

        char* current=strtok(command," ");

        if(strcmp(current,"STOP")==0){
            printf("STOP: \n");
            doStop(msg,desc,serverDesc);
            exit(0);
        }
        else if(strcmp(current,"LIST")==0){
            printf("LIST: \n");
            doList(msg,desc,serverDesc);
        }
        else if(strcmp(current,"ALL")==0){
            printf("ALL: \n");
            current=strtok(NULL," ");
            strncpy(msg->msgText,current,256);
//            msg->msgText=current;
            doAll(msg,desc,serverDesc);

        }
        else if(strcmp(current,"ONE")==0){
            printf("ONE: \n");
            current=strtok(NULL," ");
            msg->toClient=atoi(current);
            current=strtok(NULL," ");
            strncpy(msg->msgText,current,256);
//            msg->msgText=current;
            doOne(msg,desc,serverDesc);
        }
        free(msg);
    }
}