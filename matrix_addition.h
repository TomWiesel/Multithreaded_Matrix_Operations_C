#ifndef VECTOR_OPERATIONS
#define MAX_DEPTH 2 
#define VECTOR_OPERATIONS

struct arg{
    long size;
    long depth;
    int* posA;
    int* posB;
    int* result;
};

extern int* matrixA;
extern int* matrixB;
extern int* result;
extern const long size;
extern const long depth;
extern int exponent;

inline double timedifference(struct timeval start, struct timeval end){
     return (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f;
}

inline long max(long a, long b){
     return a > b ? a : b;
}

void* add_matrix_parallel(void*);
void print_matrix(int*, long n_size);
#endif

