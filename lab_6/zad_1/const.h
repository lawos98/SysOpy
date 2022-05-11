#include <time.h>
#include <sys/ipc.h>


#ifndef CONST
#define CONST

#define MAXUSERS 256

#define STOP 1
#define LIST 2
#define ALL 3
#define ONE 4
#define INIT 5

struct Message{
    long int msgType;
    char msgText[256];
    int fromClient;
    int toClient;
    struct tm time;
    key_t queueKey;
};

#endif