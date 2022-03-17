#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include "lib.h"

#ifdef DYNAMIC
    #include <dlfcn.h>
#endif


clock_t startTime,endTime;
struct tms timerStartTms,timerEndTms;
void startTimer(){
    startTime = times(&timerStartTms);
}
void endTimer(){
    endTime=times(&timerEndTms);
}
void printToFile(char command[]){
    double realTime=(double)(endTime-startTime)/sysconf(_SC_CLK_TCK);
    double userTime=(double)(timerEndTms.tms_cutime-timerStartTms.tms_cutime)/sysconf(_SC_CLK_TCK);
    double sysTime=(double)(timerEndTms.tms_cstime-timerStartTms.tms_cstime)/sysconf(_SC_CLK_TCK);
    printf("realTime: %f;userTime: %f;sysTime: %f;command: %s \n",realTime,userTime,sysTime,command);

}

int countArgs(int startIndex,int argc,char** argv){
    if(startIndex+1==argc)return -1;
    int count=1;
    while(startIndex+count<argc && strcmp(argv[startIndex+count],"create_table")!=0
                                && strcmp(argv[startIndex+count],"wc_files")!=0
                                && strcmp(argv[startIndex+count],"remove_block")!=0){
        count+=1;
    }
    return count-1;
}

int main(int argc,char** argv) {
    #ifdef DYNAMIC
        void *handle =dlopen("./liblib.so",RTLD_LAZY);
        struct ArrayOfBlocks* (*createArrayOfBlocks)(int)=dlsym(handle,"createArrayOfBlocks");
        void (*wcFiles)(char*)=dlsym(handle,"wcFiles");
        int (*createBlock)(struct ArrayOfBlocks*)=dlsym(handle,"createBlock");
        void (*removeBlock)(struct ArrayOfBlocks*,int)=dlsym(handle,"removeBlock");
    #endif
    struct ArrayOfBlocks* array=NULL;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"create_table")==0){
            int numArgs=countArgs(i,argc,argv);
            if(numArgs!=1){
                printf("Incorrect number of arguments to create_table\n");
                exit(100);
            }
            i+=1;
            if(array==NULL){
                if(atoi(argv[i])<=0){
                    printf("No positive number of size table\n");
                    exit(101);
                }
                startTimer();
                array= createArrayOfBlocks(atoi(argv[i]));
                endTimer();
                char* command=calloc(20+strlen(argv[i]),sizeof(char));
                strcpy(command,"create array size of ");
                strcat(command,argv[i]);
                printToFile(command);
            }
            else{
                printf("Table has already been created \n");
                exit(102);
            }
        }
        else if(strcmp(argv[i],"wc_files")==0){
            if(array==NULL){
                printf("Table has not yet been created \n");
                exit(200);
            }
            int fileCount=countArgs(i,argc,argv);
            if(fileCount<1){
                printf("Incorrect number of arguments to wc_files\n");
                exit(201);
            }
            int charCount=0;
            for(int index=1;index<=fileCount;index++){
                charCount+=strlen(argv[i+index]);
            }
            char* files= calloc(charCount+fileCount-1,sizeof(char));
            i+=1;
            strcpy(files,argv[i]);
            for(int index=1;index<fileCount;index++){
                strcat(files," ");
                i+=1;
                strcat(files,argv[i]);
            }
            startTimer();
            wcFiles(files);
            createBlock(array);
            endTimer();
            char* command=calloc(20+strlen(files),sizeof(char));
            strcpy(command,"wc files");
            strcat(command,files);
            printToFile(command);
            free(files);
        }
        else if(strcmp(argv[i],"remove_block")==0){
            if(array==NULL){
                printf("Table has not yet been created \n");
                exit(300);
            }
            if(countArgs(i,argc,argv)!=1){
                printf("Incorrect number of arguments to remove_block\n");
                exit(301);
            }
            if(atoi(argv[i])<0 || atoi(argv[i])>=array->numBlocks){
                printf("Incorrect number of index to remove_block\n");
                exit(302);
            }
            i+=1;
            startTimer();
            removeBlock(array,atoi(argv[i]));
            endTimer();

            char* command=calloc(20+strlen(argv[i]),sizeof(char));
            strcpy(command,"remove block at ");
            strcat(command,argv[i]);
            printToFile(command);
        }
    }
#ifdef DYNAMIC
    dlclose(handle);
#endif
    return 0;
}
