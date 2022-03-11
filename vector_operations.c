#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#define MAX_DEPTH 2 

void* addMatrix(void*);

struct args{
    long unsigned size;
    long unsigned depth;
    long unsigned x,y;
};

int* matrixA;
int* matrixB;
int* result;
long unsigned size;

inline long unsigned max(long unsigned a, long unsigned b){
    return a > b ? a : b;
}

inline double timedifference(struct timeval start, struct timeval end){
    return (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f;
}

int main(){
    
    size =  1<<14;
    matrixA =(int*)malloc(sizeof(int)*size*size); 
    matrixB =(int*)malloc(sizeof(int)*size*size); 
    result = (int*)malloc(sizeof(int)*size*size); 

    for(long unsigned o = 0; o < size*size; o++){
        *(matrixA+o) = o;
        *(matrixB+o) = o;
    }
    struct timeval start, end; 
    gettimeofday(&start,NULL);
    struct args arg = {size, 1<<8, 0,0};
    addMatrix(&arg);
    gettimeofday(&end, NULL);
    printf("%lf\n",timedifference(start, end));
    
   return EXIT_SUCCESS;
}

//@params: size, depth, matrixA, matrixB
void* addMatrix(void* params){
    struct args local = *(struct args*) params;
    if(max(MAX_DEPTH, local.depth) < local.size){
        pthread_t t_ids[4];
        long unsigned n_size = local.size/2;
        struct args arg[] = {
            {n_size, local.depth, local.x, local.y},
            {n_size, local.depth, local.x+n_size, local.y},
            {n_size, local.depth, local.x, local.y+n_size},
            {n_size, local.depth, local.x+n_size, local.y+n_size},
        };
        pthread_create(&t_ids[0], NULL, addMatrix, &arg[0]);
        pthread_create(&t_ids[1], NULL, addMatrix, &arg[1]);
        pthread_create(&t_ids[2], NULL, addMatrix, &arg[2]);
        pthread_create(&t_ids[3], NULL, addMatrix, &arg[3]);
        pthread_join(t_ids[0], NULL);
        pthread_join(t_ids[1], NULL);
        pthread_join(t_ids[2], NULL);
        pthread_join(t_ids[3], NULL);
        
    }else{
        long unsigned index;
        long unsigned offset = local.y*size + local.x;
        for(long unsigned i = 0; i < local.size; i++){
            for(long unsigned o = 0; o < local.size; o++){
                index = i * size + o;
                *(result + index+offset) = *(matrixA + index+offset) + *(matrixB + index+offset); 
            }
        }
    }
    return NULL;
}
