#include "const.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/ipc.h>
#include <mqueue.h>

char* clientQueue[MAXUSERS];

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
    clientQueue[msg->fromClient]=NULL;
}

void doList(struct Message* msg){
    printf("LIST:\n");
    char* temp=calloc(256,sizeof(char));
    char* msgText=calloc(256,sizeof(char));
    strcpy(msgText,"");
    for(int i=0;i<MAXUSERS;i++){
        if(clientQueue[i]!=NULL){
            strcpy(temp,"");
            sprintf(temp,"%d \n",i);
            strcat(msgText,temp);
        }
    }
    strncpy(msg->msgText,msgText,256);
    printf("\nList: \n%s\n ",msg->msgText);
//    msg->msgText=msgText;
    mqd_t clientQueueDESC=mq_open(clientQueue[msg->fromClient],O_RDWR);
    mq_send(clientQueueDESC,(char*)&msg,sizeof(struct Message),0);
    mq_close(clientQueueDESC);
    free(temp);
}

void doAll(struct Message* msg){
    printf("ALL:\n");
    for(int i=0;i<MAXUSERS;i++){
        if(clientQueue[i]!=NULL){
            mqd_t clientQueueDESC=mq_open(clientQueue[i],O_RDWR);
            mq_send(clientQueueDESC,(char*)&msg,sizeof(struct Message),0);
            mq_close(clientQueueDESC);
        }
    }
}

void doOne(struct Message* msg){
    printf("ONE:\n");
    mqd_t clientQueueDESC=mq_open(clientQueue[msg->toClient],O_RDWR);
    mq_send(clientQueueDESC,(char*)&msg,sizeof(struct Message),0);
    mq_close(clientQueueDESC);
}

void doInit(struct Message* msg){
    int newId=0;
    while(clientQueue[newId]!=NULL){
        newId++;
    };
    msg->fromClient=newId;
    printf("newid: %d",newId);
    strcpy(clientQueue[newId],msg->queueKey);
    mqd_t clientQueueDESC=mq_open(msg->queueKey,O_RDWR);
    mq_send(clientQueueDESC,(char*)&msg,sizeof(struct Message),0);
    mq_close(clientQueueDESC);
    printf("Init for user %d with queue key %s",newId,clientQueue[newId]);
}

int main(){
    for(int i=0;i<MAXUSERS;i++){
        clientQueue[i]=NULL;
    }
    mq_unlink("/Server");
    struct mq_attr attr;
    attr.mq_maxmsg=MAXUSERS;
    attr.mq_msgsize=sizeof(struct Message);
    mqd_t serverQueue=mq_open("/Server",O_RDWR|O_CREAT,0666,&attr);

    struct Message* newMsg=calloc(1,sizeof(struct Message));
    printf("Server ON\n");

    while(1){
        mq_receive(serverQueue,(char*)&newMsg,sizeof(struct Message),NULL);
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