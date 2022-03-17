#ifndef lib
#define lib

struct Block{
    int numLines;
    int numWords;
    int numChars;
};

struct ArrayOfBlocks{
    int numBlocks;
    int maxBlockIndex;
    struct Block** blocks;
};

struct ArrayOfBlocks* createArrayOfBlocks(int numBlocks);

void wcFiles(char* files);

int createBlock(struct ArrayOfBlocks* array);

void removeBlock(struct ArrayOfBlocks* array,int blockIndex);

void printArray(struct ArrayOfBlocks* array);

#endif