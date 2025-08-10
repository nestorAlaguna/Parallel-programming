#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>

/* Global variable:  accessible to all threads */
int thread_count,n;  
double a,b;
double sum;
int flag;

void exit_with_error(const char *message)
{
    printf("ERROR: %s\n", message);
    exit(1);
}

double approx_function(double x)
{
	return 3*x*x*x*x*x - 4*x*x*x*x + 5*x*x*x - 6*x*x + 7*x - 8;
}

void *trap(void* rank) {
   long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 
	double local_a, local_b, local_n, local_est, h, c;
	
	h=(b-a)/n;
	local_n = n/thread_count;
	local_a = a + my_rank*local_n*h;
	local_b = local_a+local_n*h;

	local_est = (approx_function(local_a) +approx_function(local_b))/2;
	 
	int i = 0;
	while(i<local_n)
	{
		c = i*h + local_a;
		local_est += approx_function(c);
		i++;
	}
	local_est = local_est*h;

 	//busy waiting
	while (flag !=my_rank);
	sum +=local_est;
	flag = (flag+1) % thread_count;

   return NULL;
}  

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 
	double elapsed=0;
   /* Get number of threads and trapezoids from command line */
 	thread_count=strtol(argv[1],NULL,10);
	n=strtol(argv[2],NULL,10);
	

   thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
	
	flag = 0;

	struct timespec start,finish;

	clock_gettime(CLOCK_MONOTONIC, &start);
	
	printf("Enter a & b\n");
	scanf("%lf%lf",&a,&b);


   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          trap, (void*) thread);  

	
 
   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

	clock_gettime(CLOCK_MONOTONIC, &finish);	
	elapsed = (finish.tv_sec-start.tv_sec);
	elapsed += (finish.tv_nsec-start.tv_nsec)/1000000000.0;
	printf("Busy waiting implementation of trapezoids approximation.\n");	
	printf("With n = %d trapezoids, the estimated area of function 3x^5 - 4x^4 + 5x^3 - 6x^2 + 7x -8 from %lf to %lf is  %f\n",n,a,b,sum);
	printf("  Elapsed time = %.5f seconds \n", elapsed);

 	free(thread_handles);
   return 0;
} 

