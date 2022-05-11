#include "const.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/ipc.h>

key_t clientQueue[MAXUSERS];

void saveLog(struct Message* msg){
    FILE* logFile=fopen("log.txt","a");
    int type=msg->msgType;
    if(type==STOP){
        fprintf(logFile,"Stop client %d \n",msg->fromClient);
    }
    else if(type==LIST){
        fprintf(logFile,"List client %d \n",msg->fromClient);
    }
    else if(type==ALL){
        fprintf(logFile,"Msg \"%s\" to All from client %d \n",msg->msgText,msg->fromClient);
    }
    else if(type==ONE){
        fprintf(logFile,"Msg \"%s\" to One from client %d to client %d\n",msg->msgText,msg->fromClient,msg->toClient);
    }
    else if(type==INIT){
        fprintf(logFile,"Init client %d \n",msg->fromClient);
    }
    struct tm time=msg->time;
    fprintf(logFile, "Time: %d-%02d-%02d %02d:%02d:%02d\n",
            time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    fprintf(logFile,"\n|============================================|\n\n");
    fclose(logFile);
}
void doStop(struct Message* msg){
    printf("STOP:\n");
    clientQueue[msg->fromClient]=-1;
}

void doList(struct Message* msg){
    printf("LIST:\n");
    char* temp=calloc(256,sizeof(char));
    char* msgText=calloc(256,sizeof(char));
    strcpy(msgText,"");
    for(int i=0;i<MAXUSERS;i++){
        if(clientQueue[i]!=-1){
            strcpy(temp,"");
            sprintf(temp,"%d \n",i);
            strcat(msgText,temp);
        }
    }
    int clientQueueId=msgget(clientQueue[msg->fromClient],0);
    strncpy(msg->msgText,msgText,256);
//    msg->msgText=msgText;
    msgsnd(clientQueueId,msg, sizeof(struct Message),0);
    printf("\nList: \n%s\n ",msg->msgText);
    free(temp);
}

void doAll(struct Message* msg){
    printf("ALL:\n");
    for(int i=0;i<MAXUSERS;i++){
        if(clientQueue[i]!=-1){
            int clientQueueId=msgget(clientQueue[i],0);
            msgsnd(clientQueueId,msg, sizeof(struct Message),0);
        }
    }
}

void doOne(struct Message* msg){
    printf("ONE:\n");
    int clientQueueId=msgget(clientQueue[msg->toClient],0);
    msgsnd(clientQueueId,msg, sizeof(struct Message),0);
}

void doInit(struct Message* msg){
    int newId=0;
    while(clientQueue[newId]!=-1){
        newId++;
    };
    msg->fromClient=newId;
    clientQueue[newId]=msg->queueKey;
    int clientQueueId=msgget(msg->queueKey,0);
    msgsnd(clientQueueId,msg, sizeof(struct Message),0);
    printf("Init for user %d with queue key %d",newId,clientQueue[newId]);
}

int main(){
    for(int i=0;i<MAXUSERS;i++){
        clientQueue[i]=-1;
    }
    key_t queueKey= ftok(getenv("HOME"),'k');
    int serverQueue=msgget(queueKey,IPC_CREAT |0777);
    struct Message* newMsg=calloc(1,sizeof(struct Message));
    printf("Server ON with Key: %d | Id: %d \n",queueKey,serverQueue);

    while(1){
        msgrcv(serverQueue,newMsg,sizeof(struct Message),-(INIT+1),0);
        int type=newMsg->msgType;
        if(type==STOP){
            printf("Stop:\n");
            doStop(newMsg);
        }
        else if(type==LIST){
            printf("List:\n");
            doList(newMsg);
        }
        else if(type==ALL){
            printf("All:\n");
            doAll(newMsg);
        }
        else if(type==ONE){
            printf("One:\n");
            doOne(newMsg);
        }
        else if(type==INIT){
            printf("Init:\n");
            doInit(newMsg);
        }
        else if(type==-999){
            exit(0);
        }
        saveLog(newMsg);
    }
}