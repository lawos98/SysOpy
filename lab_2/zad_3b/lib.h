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
struct FTW;
struct stat;

int getInfoFile(const char *filename, const struct stat *stats,int fileflags, struct FTW *pfwt);

int getInfoNFTW(char *fileName);



#endif