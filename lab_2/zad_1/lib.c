#include "lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void convertFileSys(char* inputFileName,char* outputFileName){
    int inputFile,outputFile;
    inputFile = open(inputFileName,O_RDONLY);
    if(inputFile==-1){
        printf("Cant find input file\n");
        exit(201);
    }
    outputFile = open(outputFileName,O_WRONLY|O_CREAT,S_IRUSR,S_IWUSR);
    if(outputFile==-1){
        printf("Cant find output file\n");
        exit(202);
    }
    char currentChar;
    char whitespace=' ';
    int spaceCounter=0;
    while(read(inputFile,&currentChar,1)==1){
        write(outputFile,&currentChar,1);
        while(currentChar=='\n'){
            spaceCounter=0;
            while(read(inputFile,&currentChar,1)==1 && currentChar==whitespace){
                spaceCounter++;
            }
            if(currentChar!='\n'){
                for(int i=0;i<spaceCounter;i++){
                    write(outputFile,&whitespace,1);
                }
                write(outputFile,&currentChar,1);
            }
        }
    }
};

void convertFileLib(char* inputFileName,char* outputFileName){
    FILE* inputFile = fopen(inputFileName,"r");
    if(inputFile==NULL){
        printf("Cant find input file\n");
        exit(201);
    };
    FILE* outputFile = fopen(outputFileName,"w");
    if(outputFile==NULL){
        printf("Cant find output file\n");
        exit(202);
    };
    char currentChar;
    char whitespace=' ';
    int spaceCounter=0;
    while(fread(&currentChar,1,1,inputFile)==1){
        fwrite(&currentChar,1,1,outputFile);
        while(currentChar=='\n'){
            spaceCounter=0;
            while(fread(&currentChar,1,1,inputFile)==1 && currentChar==whitespace){
                spaceCounter++;
            }
            if(currentChar!='\n'){
                for(int i=0;i<spaceCounter;i++){
                    fwrite(&whitespace,1,1,outputFile);
                }
                fwrite(&currentChar,1,1,outputFile);
            }
        }
    }
};