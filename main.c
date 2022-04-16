#include <stdio.h>
#include <gsl/gsl_ieee_utils.h>
#include <gsl/gsl_blas.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

clock_t startTime,endTime;
struct tms timerStartTms,timerEndTms;
void startTimer(){
    startTime = times(&timerStartTms);
}
void endTimer(){
    endTime=times(&timerEndTms);
}
void printToFile(char* f,int size){
    double realTime=(double)(endTime-startTime)/sysconf(_SC_CLK_TCK);
    printf("%s,%d,%f\n",f,size,realTime);
}

void naive(double A[],double B[],int size){
    double C[size*size];
    for(int index=0;index<size*size;index++){
        C[index]=0;
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            for(int k=0;k<size;k++){
                C[i+j*size]+=A[i+k*size]*B[k+j*size];
            }
        }
    }
}

void better(double A[],double B[],int size){
    int C[size*size];
    for(int index=0;index<size*size;index++){
        C[index]=0;
    }
    for(int j=0;j<size;j++){
        for(int k=0;k<size;k++){
            for(int i=0;i<size;i++){
                C[i+j*size]+=A[i+k*size]*B[k+j*size];
            }
        }
    }
}
void best(double a[],double b[],int size){
    double c[size*size];
    for(int index=0;index<size*size;index++){
        c[index]=0;
    }
    gsl_matrix_view A= gsl_matrix_view_array(a,size,size);
    gsl_matrix_view B= gsl_matrix_view_array(b,size,size);
    gsl_matrix_view C= gsl_matrix_view_array(c,size,size);

    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,&A.matrix,&B.matrix,0.0,&C.matrix);
}

int main (void)
{
    for(int i=5;i<=100;i++){
        double array[i*i];
        for(int index=0;index<i*i;index++){
            array[index]=rand()%2;
        }
        for(int j=0;j<2000;j++){
            startTimer();
            naive(array,array,i);
            endTimer();
            printToFile("naive O3",i);
            startTimer();
            better(array,array,i);
            endTimer();
            printToFile("better O3",i);
            startTimer();
            best(array,array,i);
            endTimer();
            printToFile("best O3",i);
        }
    }

}
