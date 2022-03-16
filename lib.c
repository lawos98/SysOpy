#include "lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ArrayOfBlocks* createArrayOfBlocks(int numberBlocks){
    struct ArrayOfBlocks* array =calloc(1,sizeof(struct ArrayOfBlocks));
    array -> numBlocks = numberBlocks;
    array -> maxBlockIndex = -1;
    array -> blocks = calloc(numberBlocks,sizeof(struct Block));

    return array;
};

void wcFiles(char* files){
    char command[14+strlen(files)];
    strcpy(command,"wc ");
    strcat(command,files);
    strcat(command," > temp.txt");
    system(command);
};

int createBlock(struct ArrayOfBlocks* array){
    FILE* file = fopen("temp.txt","r");
    int lines,words,chars;
    char* fName=NULL;
    while(fscanf(file,"%d %d %d %s",&lines,&words,&chars,fName)==4);

    struct Block* block= calloc(1,sizeof(struct Block));
    block->numLines=lines;
    block->numWords=words;
    block->numChars=chars;
    array->maxBlockIndex+=1;
    array->blocks[array->maxBlockIndex]=block;

    fclose(file);
    return array->maxBlockIndex;

};

void removeBlock(struct ArrayOfBlocks* array,int blockIndex){
    free(array->blocks[blockIndex]);
    array->blocks[blockIndex]=array->blocks[array->maxBlockIndex];
    array->blocks[array->maxBlockIndex]=NULL;
    array->maxBlockIndex-=1;
};

void printArray(struct ArrayOfBlocks* array){
    for(int i=0;i<=array->maxBlockIndex;i++){
        struct Block* block = array->blocks[i];
        printf("lines %d | words %d | chars %d \n",
               block->numLines,
               block->numWords,
               block->numChars);
    };
};