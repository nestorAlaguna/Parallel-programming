#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include <time.h>

long thread_count;
long n;
double sum;
pthread_mutex_t mutex;
void* Thread_sum(void* rank);

int main(int argc, char* argv[]){

    long thread;
    pthread_t* thread_handles;
    double elapsed=0;

    thread_count = strtol(argv[1],NULL,10);
    n = strtol(argv[2], NULL, 10);
    thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    struct timespec start,finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (thread = 0; thread < thread_count; thread++)
    pthread_create(thread_handles+thread,NULL, Thread_sum, (void*)thread);

    for (thread =0; thread < thread_count; thread++)
    pthread_join(*(thread_handles+thread),NULL);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec-start.tv_sec);
    elapsed += (finish.tv_nsec-start.tv_nsec)/1000000000.0;

    sum = 4.0*sum;
    printf("For n = %ld terms,\n", n);
    printf(" The Multi-threaded estimation of pi = %.15f\n", sum);
    printf(" Elapsed time = %.5f seconds \n", elapsed);
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    return 0;
}

void* Thread_sum(void* rank){
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long i;
    long my_n = n/thread_count;
    long my_first_i = my_n*my_rank;
    long my_last_i = my_first_i+my_n;
    if (my_first_i % 2 ==0)
    factor = 1.0;
    else
    factor = -1.0;
    for (i = my_first_i; i < my_last_i; i++, factor = -factor){

    pthread_mutex_lock(&mutex);

    sum +=factor/(2*i+1);
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
} /* Thread_sum */
