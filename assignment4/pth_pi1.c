#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// Global variables that every thread can access
long thread_count;
long long no_atempts,
long sum;
double pi;
pthread_mutex_t mutex;
//void *number_calc(void *rank);
void *number_calc(void *rank){

long int num_cir = 0;
long int local_n;
double x, y, dist;

local_n = no_atempts/thread_count;

for (int i=0; i<local_n; i++) {
x = (double)rand() / RAND_MAX*2 - 1;
y = (double)rand() / RAND_MAX*2 - 1;

//check if in circle and add up addition is critical
dist= x * x + y * y;

if (dist <= 1)
num_cir++;
}

pthread_mutex_lock(&mutex);
sum += num_circle;
pthread_mutex_unlock(&mutex);

return NULL;
}

int main(int argc, char *argv[]){

long thread;
pthread_t *thread_handles;

// Get the number of threads from the command line
thread_count = strtol(argv[1],NULL,10);
no_atempts = strtol(argv[2], NULL, 10);

thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));

for (thread = 0; thread < thread_count; thread++)
    pthread_create(thread_handles+thread,NULL, number_calc, (void*)thread);
for (thread = 0; thread < thread_count; thread++)
    pthread_join(*(thread_handles+thread), NULL);

//estimate pi
pi = 4.0*sum/no_atempts;

printf("Number of attempts = %lld ,\n", no_atempts);
printf("the Multi-threaded estimate is= %.15f\n", pi);
pthread_mutex_destroy(&mutex);
free(thread_handles);
return 0;
}



