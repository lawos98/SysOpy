#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h>

int width;
int height;
int ** inputImage;
int ** outputImage;
int threadsCount;
pthread_t * threads;

void *handlerNumber(void * args){
    clock_t  t[2];
    t[0] = clock();
    int n = * (int *) args;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(inputImage[i][j] >= 256 / threadsCount * n && inputImage[i][j] < 256 / threadsCount * (n + 1)){
                outputImage[i][j] = 255 - inputImage[i][j];
            }
        }
    }
    t[1] = clock();
    double* time = (double*) calloc(1, sizeof(double));
    *time = (double)(t[1] - t[0]) / sysconf(_SC_CLK_TCK);
    pthread_exit(time);
}

void *handlerBlock(void * args){
    clock_t  t[2];
    t[0] = clock();
    int n = * (int *) args;
    for(int j = (n * width) / threadsCount; j <= (n + 1) * width / threadsCount; j++){
        for(int i = 0; i < height; i++){
            outputImage[i][j] = 255 - inputImage[i][j];
        }
    }
    t[1] = clock();
    double* time = (double*) calloc(1, sizeof(double));
    *time = (double)(t[1] - t[0]) / sysconf(_SC_CLK_TCK);
    pthread_exit(time);
}

int main (int argc, char* argv[]){

    char* id = (char*) calloc(50, sizeof(char));

    threadsCount = atoi(argv[1]);
    threads = (pthread_t*) calloc(threadsCount, sizeof (pthread_t));
    int checkIfNumber;
    if(strcmp((argv[2]), "numbers") == 0){
        checkIfNumber = 1;
    }
    else{
        checkIfNumber = 0;
    }

    char *fileNameInput=argv[3];

    FILE *pic = fopen(fileNameInput, "r");

    fscanf(pic, "%s", id);
    fscanf(pic, "%d %d", &width, &height);
    inputImage = (int**) calloc(height, sizeof(int*));
    outputImage = (int**) calloc(height, sizeof(int*));
    for(int i = 0; i < height; i++){
        inputImage[i] = ( int*) calloc(width, sizeof(int));
        outputImage[i] = (int*) calloc(width, sizeof(int));
    }
    for (int i = 0; i < height; i++){
        for(int j = 0; j < width; j ++){
            fscanf(pic, "%d", &inputImage[i][j]);
        }
    }

    clock_t  t[2];
    t[0] = clock();

    for(int i = 0; i < threadsCount; i++){
        int * thread = (int*) malloc(sizeof(int));
        *thread = i;
        if( checkIfNumber == 1){
            pthread_create(&threads[i], NULL,handlerNumber, thread);
        }
        else{
            pthread_create(&threads[i], NULL, handlerBlock, thread);
        }
    }

    double * singleTime;
    for(int i = 0; i < threadsCount; i++){
        pthread_join(threads[i], (void*)&singleTime);
        printf("Thread: %d Time: %f\n", i, *singleTime);
    }

    t[1] = clock();
    double time = (double)(t[1] - t[0]) / sysconf(_SC_CLK_TCK);
    printf("All time: %f \n", time);

    char *fileNameOutput=argv[4];;

    FILE* output = fopen(fileNameOutput, "w");
    fprintf(output, "%s\n%d %d\n%d\n",id,width, height, 255);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fprintf(output, "%d ", outputImage[i][j]);
        }
        fprintf(output, "\n");
    }
    return 0;
}