#include "lib.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc,char** argv) {
    if(argc<4){
        printf("missing arguments");
        exit(1);
    }
    createProcess(argv[1],argv[2],atoi(argv[3]));
}
