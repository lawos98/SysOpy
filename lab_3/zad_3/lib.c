#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>


int checkIfInside(char *path, char *searchValue) {
    int searchValueLen = strlen(searchValue);
    FILE *inputFile = fopen(path, "r");
    char currentChar;
    int searchIndex = 0;
    while (fread(&currentChar, 1, 1, inputFile) == 1) {
        if (searchValue[searchIndex] == currentChar) {
            searchIndex++;
        } else {
            searchIndex = 0;
        }
        if (searchValueLen == searchIndex) {
            return 1;
        }
    }
    return 0;
}

void createProcess(char *path, char *searchValue, int depth){
    DIR *dir = opendir(path);
    struct dirent *fileInside;
    fileInside = readdir(dir);
    char* filePath= calloc(1024,sizeof(char));
    char* rootFilePath= calloc(1024,sizeof(char));
    strcpy(rootFilePath,path);
    while (fileInside != NULL && depth>=0) {
        if (strcmp(fileInside->d_name, ".") == 0 || strcmp(fileInside->d_name, "..") == 0) {
            fileInside = readdir(dir);
            continue;
        }
        strcpy(filePath,rootFilePath);
        strcat(filePath, "/");
        strcat(filePath, fileInside->d_name);
        struct stat stats;
        if(stat(filePath,&stats)<0){
            printf("can't stat file");
            exit(-303);
        }
        if (S_ISREG(stats.st_mode) && checkIfInside(filePath, searchValue)) {
            printf("%d %s",getpid(),filePath);
        }
        else if (S_ISDIR(stats.st_mode)) {
            if(fork()==0){
                closedir(dir);
                dir=opendir(filePath);
                strcpy(rootFilePath,filePath);depth--;
            }
        }
        fileInside = readdir(dir);
    }
    free(filePath);
    closedir(dir);
}