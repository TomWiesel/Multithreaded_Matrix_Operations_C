#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "vector_operations.h"


void mul_matrix_seq(int*, int*, int*, long);

int* matrixA_g;
int* matrixB_g;
int* result_g;
const long abs_size = 1 << 12;

int main(){

    matrixA_g = malloc(sizeof(int) *abs_size*abs_size);
    matrixB_g = malloc(sizeof(int) * abs_size*abs_size);
    result_g = malloc(sizeof(int) * abs_size*abs_size);

    for(long o = 0; o < abs_size; o++){
        *(matrixA_g+o) = o%10;
        *(matrixB_g+o) = o%10;
    }
    

    struct timeval start, end;
    gettimeofday(&start, NULL);
    mul_matrix_seq(matrixA_g, matrixB_g, result_g,abs_size);
    gettimeofday(&end, NULL);
    printf("%lf------\n", timedifference(start,end));

    return EXIT_SUCCESS;
}

void mul_matrix_seq(int* matrixA_l, int* matrixB_l, int* result_l, long size_l){
    for(long i = 0; i < size_l; i++){
        for(long o = 0; o < size_l; o++){
            *(result_l+i*size_l+o) = 0;
            for(long j = 0; j < size_l; j++){
                *(result_l+i*size_l+o) += *(matrixA_l+i*size_l+j) * *(matrixB_l + j*size_l+o);
            }
        }
    }
}
