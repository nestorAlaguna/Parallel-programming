#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include <time.h>
#include<semaphore.h>

long thread_count;
long long n;
sem_t* sem;
double sum;

void* Thread_sum(void* rank);


int main(int argc, char* argv[]){

	long 		thread;    			/* Use long in case of a 64-bit system*/
	pthread_t*	thread_handles;
	double elapsed=0;

	/*Get the number of threads from the command line */
	thread_count = strtol(argv[1],NULL,10);
	n  =  strtol(argv[2], NULL, 10); 

	thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));
	
	
	struct timespec start,finish;

	clock_gettime(CLOCK_MONOTONIC, &start);	
	
	sem = malloc(thread_count*sizeof(sem_t));
	
	for (thread = 0; thread < thread_count; thread++)
		pthread_create(thread_handles+thread,NULL, Thread_sum, (void*)thread);    	/* or &thread_handles[thread]*/
	
	for (thread = 0; thread < thread_count; thread++)
	sem_init(&sem[thread],0,1);

	for (thread =0; thread <  thread_count; thread++) 
		 		pthread_join(*(thread_handles+thread),NULL);			/* or thread_handles[thread]  */
	
	clock_gettime(CLOCK_MONOTONIC, &finish);	
	elapsed = (finish.tv_sec-start.tv_sec);
	elapsed += (finish.tv_nsec-start.tv_nsec)/1000000000.0;
		
	
	sum = 4.0*sum;
	printf("Semaphores implmentation.\n");
	printf("With n = %lld tosses, the multi-thread estimate of pi = %f\n", n, sum);;
	printf("  Elapsed time = %.5f seconds \n", elapsed);
	for (thread=0;thread<thread_count;thread++){
	sem_destroy(&sem[thread]);		
	}
	
	free(sem);
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
		my_sum +=factor/(2*i+1);	

	sem_wait(&sem[my_rank]);
	sum += my_sum;
	sem_post(&sem[my_rank]);	

	return NULL;

}  /* Thread_sum */



	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
		
	
	
	
