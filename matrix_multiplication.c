#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include "matrix_addition.h"

#define MAX_DEPTH 2

void* mul_matrix_parallel(void*);

static int split_orderA[][2] = {
    {0,0},
    {0,1},
    {0,0},
    {0,1},
    {1,0},
    {1,1},
    {1,0},
    {1,1}
};

static int split_orderB[][2] = {
    {0,0},
    {1,0},
    {0,1},
    {1,1},
    {0,0},
    {1,0},
    {0,1},
    {1,1}
};

int main(){
    matrixA = (int*)malloc(sizeof(int)*size*size);
    matrixB = (int*)malloc(sizeof(int)*size*size);
    result = (int*)malloc(sizeof(int)*size*size);
      for(long o = 0; o < size*size; o++){
         *(matrixA+o) = rand()%10;
         *(matrixB+o) = rand()%10;
     }
    print_matrix(matrixA,size);
    printf("\n------------\n");
    print_matrix(matrixB,size);
    struct timeval start, end;
    gettimeofday(&start, NULL);  
    struct arg* argu = malloc(sizeof(struct arg));
    argu->size = size;
    argu->depth = depth;
    argu->posA = matrixA;
    argu->posB = matrixB;
    argu->result = result;

    mul_matrix_parallel(argu);
    gettimeofday(&end, NULL);
    printf("%lf ----\n", timedifference(start, end));
    print_matrix(result, size);
    return EXIT_SUCCESS;
}



void* mul_matrix_parallel(void* args){
    struct arg* local = args; 
    if(local->depth < local->size){
        long n_size = local->size>>1;
        long dimension = n_size*n_size;
        int* results =  malloc(sizeof(int)*dimension*8);
        pthread_t t_ids[8];

        struct arg* argu = malloc(sizeof(struct arg)*8);
        for(int i = 0; i < 8; i++){
           (argu+i)->size =  n_size;
           (argu+i)->depth = local->depth;
           (argu+i)->posA = local->posA+size*n_size*split_orderA[i][0]+n_size*split_orderA[i][1];
           (argu+i)->posB = local->posB+size*n_size*split_orderB[i][0]+n_size*split_orderB[i][1];
           (argu+i)->result = results + i * dimension;
        }

        for(int i = 0; i < 8; i++){
            pthread_create(&t_ids[i], NULL, mul_matrix_parallel, argu+i);
        }
        for(int i = 0; i < 8; i++){
            pthread_join(t_ids[i], NULL);
        }

        for(int i = 0; i <= 1; i++){
            for(int o = 0; o < n_size; o++){
                for(int j = 0; j < n_size; j++){
                    *(local->result+n_size+j+o*local->size+i*n_size*local->size) = *(results+(2+(i<<2))*dimension+j+o*n_size) + *(results+(3+(i<<2))*dimension+j+o*n_size);
                }
            }
        }
        for(int i = 0; i <= 1; i++){
            for(int o = 0; o < n_size; o++){
                for(int j = 0; j < n_size; j++){
                    *(local->result+j+o*local->size+i*n_size*local->size) = *(results+(i<<2)*dimension+j+o*n_size) + *(results+(1+(i<<2))*dimension+j+o*n_size);
                }
            }
        }
        free(argu);
    }else{
        for(long i = 0; i < local->size; i++){
            for(long o = 0; o < local->size; o++){
                *(local->result+i*local->size+o) = 0;
                for(long j = 0; j < local->size; j++){
                    *(local->result+i*local->size+o) += *(local->posA+i*size+j) * *(local->posB+j*size+o);
                }
            }
        }
    }
    return NULL;
}

