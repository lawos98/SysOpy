#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>

int main(int argc, char **argv) {
    if(argc<5)return 1;

    FILE* fifo=fopen(argv[1], "w");
    int lineNumber=atoi(argv[2]);
    FILE* file=fopen(argv[3], "r");
    int numberToRead=atoi(argv[4]);

    sleep(5);

    char line[numberToRead];
    while(fread(line,sizeof(char),numberToRead,file)==numberToRead){
        flock(fileno(fifo),LOCK_EX);
        fwrite(&lineNumber,sizeof(int),1,fifo);
        fwrite(line,sizeof(char),numberToRead,fifo);
        flock(fileno(fifo),LOCK_UN);
    }


    return 0;
}