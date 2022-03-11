#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

inline double timedifference(struct timeval start, struct timeval end){
     return (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f;
}

int main(){
    long unsigned size = 1<<14;
    int* matrixA = (int*)malloc(sizeof(int)*size*size);
    int* matrixB = (int*)malloc(sizeof(int)*size*size);
    int* result = (int*)malloc(sizeof(int)*size*size);
    
    struct timeval start, end;
    for(long unsigned i = 0; i < size*size; i++){
        *(matrixA+i) = i;
        *(matrixB+i) = i;
    }

    gettimeofday(&start, 0);
    for(long unsigned i = 0; i < size*size; i++){
         *(result+i) = *(matrixA+i) + *(matrixB+i);
    }
    gettimeofday(&end, 0);
    printf("%lf\n",timedifference(start,end)); 
    return EXIT_SUCCESS;
}
