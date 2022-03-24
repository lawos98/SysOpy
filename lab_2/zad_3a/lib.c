#include "lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

void searchFiles(char* path,struct fileInfo* fileInfo){
    DIR* dir = opendir(path);
    if(dir==NULL){
        printf("Dir dont exist \n");
        exit(-301);
    }
    struct dirent* fileInside;
    fileInside=readdir(dir);
    while(fileInside!=NULL){
        if(strcmp(fileInside->d_name,".")==0 || strcmp(fileInside->d_name,"..")==0 ){
            fileInside=readdir(dir);
            continue;
        }
        char filePath[1024];
        strcpy(filePath,path);
        strcat(filePath,"/");
        strcat(filePath,fileInside->d_name);
        struct stat stats;
        if(stat(filePath,&stats)<0){
            printf("can't stat file");
            exit(-303);
        }
        if(S_ISDIR(stats.st_mode)){
            searchFiles(filePath,fileInfo);
        }
        printf("<============================> \n");
        printf("File name: %s \n",fileInside->d_name);
        char realPathFile[1024];
        printf("Path: %s", realpath(filePath,realPathFile));
        printf("Number of Nlinks: %ld \n",stats.st_nlink);
        printf("File type: ");
        if(S_ISREG(stats.st_mode)){
            fileInfo->fileCount++;
            printf("File \n");
        }
        else if(S_ISDIR(stats.st_mode)){
            fileInfo->dirCount++;
            printf("Dir \n");
        }
        else if(S_ISCHR(stats.st_mode)){
            fileInfo->charDevCount++;
            printf("Char Dev \n");
        }
        else if(S_ISBLK(stats.st_mode)){
            fileInfo->blockDevCount++;
            printf("Block Dev \n");
        }
        else if(S_ISFIFO(stats.st_mode)){
            fileInfo->fifoCount++;
            printf("Fifo \n");
        }
        else if(S_ISBLK(stats.st_mode)){
            fileInfo->slinkCount++;
            printf("Slink \n");
        }
        else if(S_ISSOCK(stats.st_mode)){
            fileInfo->sockCount++;
            printf("Sock \n");
        }
        else{
            printf("Unknown \n");
        }
        printf("File size: %ld \n",stats.st_size);
        printf("Last acces: %ld \n",stats.st_atime);
        printf("Last modification: %ld \n",stats.st_mtime);
        fileInside=readdir(dir);
    }
}
void getInfoOpendir(char* fileName){
    struct fileInfo* info=calloc(1,sizeof(struct fileInfo));
    info->fileCount=0;
    info->dirCount=0;
    info->charDevCount=0;
    info->blockDevCount=0;
    info->fifoCount=0;
    info->slinkCount=0;
    info->sockCount=0;
    searchFiles(fileName,info);
    printf("|============================| \n");
    printf("fileCount: %d \n",info->fileCount);
    printf("dirCount: %d \n",info->dirCount);
    printf("charDevCount: %d \n",info->charDevCount);
    printf("blockDevCount: %d \n",info->blockDevCount);
    printf("fifoCount: %d \n",info->fifoCount);
    printf("slinkCount: %d \n",info->slinkCount);
    printf("sockCount: %d \n",info->sockCount);
}
