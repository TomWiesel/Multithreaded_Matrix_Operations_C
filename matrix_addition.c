#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include "matrix_addition.h"

int* matrixA;
int* matrixB;
int* result;
const long size = 1 << 5;
const long depth = 1<<1;
int exponent;

/*
int main(){
    exponent = (int)log2(size);
    matrixA =(int*)malloc(sizeof(int)*size*size); 
    matrixB =(int*)malloc(sizeof(int)*size*size); 
    result = (int*)malloc(sizeof(int)*size*size); 

    for(long o = 0; o < size*size; o++){
        *(matrixA+o) = o%10;
        *(matrixB+o) = o%10;
    }
    struct timeval start, end; 
    gettimeofday(&start,NULL);
    struct arg* arg_local = malloc(sizeof(struct arg));
    arg_local->size = size;
    arg_local->depth = depth;
    arg_local->posA = matrixA;
    arg_local->posB = matrixB;
    arg_local->result = result;
    add_matrix_parallel(&arg_local);
    gettimeofday(&end, NULL);
    printf("%lf\n",timedifference(start, end));
    print_matrix(result, size); 
    return EXIT_SUCCESS;
}
*/

void* add_matrix_parallel(void* params){
    struct arg* local = params;
    if(local->depth < local->size){
        pthread_t t_ids[4];
        long n_size = local->size>>1;
        struct arg arg_local[4] = {
            {n_size, local->depth, local->posA, local->posB, local->result},
            {n_size, local->depth, local->posA+n_size, local->posB+n_size, local->result+n_size},
            {n_size, local->depth, local->posA+n_size*size, local->posB+n_size*size, local->result+n_size*size},
            {n_size, local->depth, local->posA+n_size*size+n_size, local->posB+n_size*size+n_size,local->result+n_size*size+n_size}
        };
        pthread_create(&t_ids[0], NULL, add_matrix_parallel, &arg_local[0]);
        pthread_create(&t_ids[1], NULL, add_matrix_parallel, &arg_local[1]);
        pthread_create(&t_ids[2], NULL, add_matrix_parallel, &arg_local[2]);
        pthread_create(&t_ids[3], NULL, add_matrix_parallel, &arg_local[3]);
        pthread_join(t_ids[0], NULL);
        pthread_join(t_ids[1], NULL);
        pthread_join(t_ids[2], NULL);
        pthread_join(t_ids[3], NULL);
    }else{
        long index;
        for(long i = 0; i < local->size; i++){
            for(long o = 0; o < local->size; o++){
                index = (i * size) + o;
                *(local->result + index) = *(local->posA + index) + *(local->posB + index); 
            }
        }
    }
   return NULL; 
}

void print_matrix(int* matrix, long n_size){
    printf("------ SIZE: %ld -----\n", n_size);
    for(long i = 0; i < n_size; i++){
        for(long o = 0; o < n_size; o++){
            printf("%d  ", *(matrix+i*size+o));
        }
        printf("\n");
    }
}
