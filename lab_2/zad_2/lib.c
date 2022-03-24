#include "lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void countFileSys(char* inputFileName,char inputChar){
    int inputFile;
    inputFile = open(inputFileName,O_RDONLY);
    if(inputFile==-1){
        printf("Cant find input file\n");
        exit(201);
    }
    char currentChar;
    int countChars=0;
    int countLines=0;
    int count=0;
    while(read(inputFile,&currentChar,1)==1){
        if(currentChar==inputChar){
            count++;
        }
        if(currentChar=='\n' && count>0){
            countLines++;
            countChars+=count;
            count=0;
        }
    }
    if(count>0) {
        countLines++;
        countChars += count;
    }
    printf("result: char %d | lines: %d ",countChars,countLines);
};

void countFileLib(char* inputFileName,char inputChar){
    FILE* inputFile = fopen(inputFileName,"r");
    if(inputFile==NULL){
        printf("Cant find input file\n");
        exit(201);
    };


    char currentChar;
    int countChars=0;
    int countLines=0;
    int count=0;
    while(fread(&currentChar,1,1,inputFile)==1){
        if(currentChar==inputChar){
            count++;
        }
        if(currentChar=='\n' && count>0){
            countLines++;
            countChars+=count;
            count=0;
        }
    }
    if(count>0) {
        countLines++;
        countChars += count;
    }
    printf("result: char %d | lines: %d ",countChars,countLines);
};