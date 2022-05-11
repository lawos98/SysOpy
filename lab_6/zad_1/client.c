#include "const.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>

void doStop(struct Message* msg,int queueId,int serverId){
    msg->msgType=STOP;
    msgsnd(serverId,msg, sizeof(struct Message),0);
    msgctl(queueId,IPC_RMID,NULL);
    exit(0);
}

void doList(struct Message* msg,int queueId,int serverId){
    msg->msgType=LIST;
    msgsnd(serverId,msg, sizeof(struct Message),0);
    msgrcv(queueId,msg,sizeof(struct Message),0,0);
    printf("msgType:\n%s \n",msg->msgText);
}
void doAll(struct Message* msg,int queueId,int serverId){
    msg->msgType=ALL;
    msgsnd(serverId,msg, sizeof(struct Message),0);
}

void doOne(struct Message* msg,int queueId,int serverId){
    msg->msgType=ONE;
    msgsnd(serverId,msg, sizeof(struct Message),0);
}

int doInit(struct Message* msg,int queueId,int serverId){
    msg->msgType=INIT;
    msgsnd(serverId,msg, sizeof(struct Message),0);
    msgrcv(queueId,msg,sizeof(struct Message),0,0);
    printf("Init for user %d with queue key %d\n",msg->fromClient,msg->queueKey);
    return msg->fromClient;
}

int main(){
    srand(time(NULL));
    key_t serverKey= ftok(getenv("HOME"),'k');
    int serverId=msgget(serverKey,0);
    key_t queueKey= ftok(getenv("HOME"),rand()%1024);
    int queueId=msgget(queueKey,IPC_CREAT |0777);
    printf("serverId=%d queueId=%d\n",serverId,queueId);
    printf("serverKey=%d queuKey=%d\n",serverKey,queueKey);
    struct Message* msgInit=calloc(1,sizeof(struct Message));
    time_t t=time(NULL);
    msgInit->time=*localtime(&t);
    msgInit->queueKey=queueKey;
    int clientId= doInit(msgInit,queueId,serverId);
    free(msgInit);

    char* command="";
    size_t length=0;
    ssize_t read;

    while(1){
        struct Message* msgRecived=calloc(1,sizeof(struct Message));
        while(msgrcv(queueId,msgRecived,sizeof(struct Message),0,IPC_NOWAIT)>=0){
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
        msg->queueKey=queueKey;

        char* current=strtok(command," ");

        if(strcmp(current,"STOP")==0){
            printf("STOP: \n");
            doStop(msg,queueId,serverId);
            exit(0);
        }
        else if(strcmp(current,"LIST")==0){
            printf("LIST: \n");
            doList(msg,queueId,serverId);
        }
        else if(strcmp(current,"ALL")==0){
            printf("ALL: \n");
            current=strtok(NULL," ");
            strncpy(msg->msgText,current,256);
//            msg->msgText=current;
            doAll(msg,queueId,serverId);

        }
        else if(strcmp(current,"ONE")==0){
            printf("ONE: \n");
            current=strtok(NULL," ");
            msg->toClient=atoi(current);
            current=strtok(NULL," ");
            strncpy(msg->msgText,current,256);
//            msg->msgText=current;
            doOne(msg,queueId,serverId);
        }
        free(msg);
    }
}