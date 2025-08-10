#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

 // Global variables that every thread can access
 long thread_count;
 long long tosses, sum;
 double pi;
 pthread_mutex_t mutex;

void *number_circle(void *rank);


int main(int argc, char *argv[])
{
    long thread;
	pthread_t *thread_handles;
//    double elapsed=0;

	// /*Get the number of threads from the command line */
	thread_count = strtol(argv[1],NULL,10);
	tosses  =  strtol(argv[2], NULL, 10);

	thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));

//    struct timespec start,finish;
//	clock_gettime(CLOCK_MONOTONIC, &start);


	for (thread = 0; thread < thread_count; thread++)
		pthread_create(thread_handles+thread,NULL, number_circle, (void*)thread);

	for (thread = 0; thread < thread_count; thread++)
		pthread_join(*(thread_handles+thread), NULL);


	pi = 4.0*sum/tosses;

//	clock_gettime(CLOCK_MONOTONIC, &finish);
//	elapsed = (finish.tv_sec-start.tv_sec);
//	elapsed += (finish.tv_nsec-start.tv_nsec)/1000000000.0;

	printf("Number of tosses = %ld terms,\n", tosses);
	printf("  Multi-threaded estimate of pi = %.15f\n", pi);

//	printf("  Elapsed time = %.5f seconds \n", elapsed);

	pthread_mutex_destroy(&mutex);  //free the mutex
	free(thread_handles);   // free the allocated memory
	return 0;

}

void *number_circle(void *rank)
{
	long int num_circle = 0;
	long int local_n;
	double x, y, distance;

	local_n = tosses/thread_count;

	for (int t=0; t<local_n; t++) {
		x = (double)rand() / RAND_MAX*2 - 1;
		y = (double)rand() / RAND_MAX*2 - 1;
		distance = x * x + y * y;
		if (distance <= 1)
		num_circle++;
}
 	pthread_mutex_lock(&mutex);
 	sum += num_circle;
	pthread_mutex_unlock(&mutex);

	return NULL;
}
