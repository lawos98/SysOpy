#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

double f(double x){
    return 4/(x*x+1);
}

double rangeToNextBlock(int numberOfBlocks,double lengthOfBlock){
    if(numberOfBlocks<=1)return 0;
    return (1-lengthOfBlock)/(numberOfBlocks-1);
}

int lengthOfInt(int x){
    int count=0;
    while(x>0){
        x/=10;
        count++;
    }
    return count;
}

void saveBlock(double x,double lengthOfBlock,int number){
    double result = f(x)*lengthOfBlock;
    char *filename=calloc(lengthOfInt(number)+5,sizeof(char));
    sprintf(filename,"w%d.txt",number);
    FILE* fResult=fopen(filename,"w");
    fprintf(fResult,"%lf",result);
    free(filename);
    fclose(fResult);
}
void readBlock(int number){
    double sum=0;
    double result=0;
    for(int i=1;i<=number;i++){
        char *filename=calloc(lengthOfInt(i)+5,sizeof(char));
        sprintf(filename,"w%d.txt",i);
        FILE* fResult=fopen(filename,"r");
        fscanf(fResult,"%lf",&result);
        sum+=result;
        free(filename);
        fclose(fResult);
    }
    printf("result: %lf\n",sum);
}
void calculateFunction(int numberOfBlocks,double lengthOfBlock){
    double currentValue=lengthOfBlock/2;
    double nextBlockValue=rangeToNextBlock(numberOfBlocks,lengthOfBlock);
    for(int i=1;i<=numberOfBlocks;i++){
        if(fork()==0) {
            saveBlock(currentValue, lengthOfBlock, i);
            exit(0);
        }
        currentValue+=nextBlockValue;
    }
    int status;
    while(wait(&status)>=0);
    readBlock(numberOfBlocks);
}