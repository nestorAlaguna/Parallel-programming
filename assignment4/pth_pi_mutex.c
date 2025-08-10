#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include <time.h>

long thread_count;
long long n;
double sum;
pthread_mutex_t mutex;
int flag;

void* Thread_sum(void* rank);

/* only executed in the main thread */
double Serial_pi(long n);

int main(int argc, char* argv[]){

	long 		thread;    			/* Use long in case of a 64-bit system*/
	pthread_t*	thread_handles;
	flag = 0;

	/*Get the number of threads from the command line */
	thread_count = strtol(argv[1],NULL,10);
	n  =  strtol(argv[2], NULL, 10); 

	thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));
	pthread_mutex_init(&mutex, NULL);

	struct timespec start,finish;

	clock_gettime(CLOCK_MONOTONIC, &start);	
	sum  = 0.0;
	
	for (thread = 0; thread < thread_count; thread++)
		pthread_create(thread_handles+thread,NULL, Thread_sum, (void*)thread);    	/* or &thread_handles[thread]*/

	for (thread =0; thread <  thread_count; thread++) 
		pthread_join(*(thread_handles+thread), NULL);					/* or thread_handles[thread]  */
	
	clock_gettime(CLOCK_MONOTONIC, &finish);	
	double elapsed = (finish.tv_sec-start.tv_sec);
	elapsed += (finish.tv_nsec-start.tv_nsec)/1000000000.0;
		
	sum = 4.0*sum;
	printf("Mutex implementation.\n");
	printf("With n = %lld tosses, the multi-thread estimate of pi = %f\n", n, sum);;
	printf("  Elapsed time = %.5f seconds \n", elapsed);

	pthread_mutex_destroy(&mutex);
	free(thread_handles);
	return 0;

}  /* main */


/*-------------------------------------------------------------------*/
void* Thread_sum(void* rank){
	
	long my_rank = (long) rank;
	double factor, my_sum = 0.0;
	long i;
	long my_n = n/thread_count;
	long my_first_i = my_n*my_rank;
	long my_last_i  = my_first_i+my_n;

	if (my_first_i % 2 ==0)
		factor = 1.0;
	else 
		factor = -1.0;

	for (i = my_first_i; i < my_last_i; i++, factor = -factor)
	{
		my_sum +=factor/(2*i+1);
	}
	pthread_mutex_lock(&mutex);
	sum +=my_sum;
	pthread_mutex_unlock(&mutex);		


	return NULL;

}  /* Thread_sum */

