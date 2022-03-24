#ifndef lib
#define lib

struct fileInfo{
    int fileCount;
    int dirCount;
    int charDevCount;
    int blockDevCount;
    int fifoCount;
    int slinkCount;
    int sockCount;
};
void getInfoOpendir(char* fileName);

void searchFiles(char* path,struct fileInfo* fileInfo);


#endif