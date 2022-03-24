#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int fileCount = 0;
int dirCount = 0;
int charDevCount = 0;
int blockDevCount = 0;
int fifoCount = 0;
int slinkCount = 0;
int sockCount = 0;

int getInfoFile(const char *filename, const struct stat *stats,int fileflags,struct FTW *pfwt) {
    printf("<============================> \n");
    printf("File name: %s \n", filename);
    char realPathFile[1024];
    printf("Path: %s", realpath(filename, realPathFile));
    printf("Number of Nlinks: %ld \n", stats->st_nlink);
    printf("File type: ");
    if (S_ISREG(stats->st_mode)) {
        fileCount++;
        printf("File \n");
    } else if (S_ISDIR(stats->st_mode)) {
        dirCount++;
        printf("Dir \n");
    } else if (S_ISCHR(stats->st_mode)) {
        charDevCount++;
        printf("Char Dev \n");
    } else if (S_ISBLK(stats->st_mode)) {
        blockDevCount++;
        printf("Block Dev \n");
    } else if (S_ISFIFO(stats->st_mode)) {
        fifoCount++;
        printf("Fifo \n");
    } else if (S_ISBLK(stats->st_mode)) {
        slinkCount++;
        printf("Slink \n");
    } else if (S_ISSOCK(stats->st_mode)) {
        sockCount++;
        printf("Sock \n");
    } else {
        printf("Unknown \n");
    }
    printf("File size: %ld \n", stats->st_size);
    printf("Last acces: %ld \n", stats->st_atime);
    printf("Last modification: %ld \n", stats->st_mtime);
    return 0;
}

int getInfoNFTW(char *fileName) {
    nftw(fileName,
         getInfoFile,
         10, FTW_PHYS);
    printf("|============================| \n");
    printf("fileCount: %d \n", fileCount);
    printf("dirCount: %d \n", dirCount);
    printf("charDevCount: %d \n", charDevCount);
    printf("blockDevCount: %d \n", blockDevCount);
    printf("fifoCount: %d \n", fifoCount);
    printf("slinkCount: %d \n", slinkCount);
    printf("sockCount: %d \n", sockCount);
    return 1;
}
