#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>

int main(int argc, char **argv) {
    if(argc<4)return 1;

    FILE* fifo=fopen(argv[1], "w");
    FILE* file=fopen(argv[2], "r");
    int numberToRead=atoi(argv[3]);

    char line[numberToRead];
    int number;
    int counter=0;
    char currentChar=' ';
    while(fread(&number,sizeof(int),1,fifo)){
        file=fopen(argv[2], "r");
        fread(line,sizeof(char),numberToRead,fifo);
        flock(fileno(fifo),LOCK_EX);
        counter=0;
        fseek(file,0,SEEK_SET);
        while(fread(&currentChar,sizeof(char),1,file)){
            if(currentChar=='\n'){
                counter++;
            }
            if(counter==number){
                fseek(file,-1,SEEK_CUR);
                char temp[1024];
                int back=fread(temp,sizeof(char),1024,file);
                fseek(file,-back,SEEK_CUR);
                fwrite(line,sizeof(char),numberToRead,file);
                fwrite(temp,sizeof(char),back,file);
            }
        }
        flock(fileno(file),LOCK_UN);
    }

    return 0;
}