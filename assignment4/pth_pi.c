#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

long thread_count;
long long n;
double sum;
double gen_max = RAND_MAX;

void* get_points(void* rank);
pthread_mutex_t mutex;

double Serial_pi(long n);

int main(int argc, char* argv[]){

	long 		thread;    			/* Use long in case of a 64-bit system*/
	pthread_t*	thread_handles;
	
	/*Get the number of threads from the command line */
	thread_count = strtol(argv[1],NULL,10);
	n  =  strtol(argv[2], NULL, 10); 

	pthread_mutex_init(&mutex, NULL);

	thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));
	sum = 0.0;

	for (thread = 0; thread < thread_count; thread++)
		pthread_create(thread_handles+thread,NULL, get_points, (void*)thread);    	/* or &thread_handles[thread]*/

	for (thread =0; thread <  thread_count; thread++) 
		pthread_join(*(thread_handles+thread), NULL);					/* or thread_handles[thread]  */

	sum = 4.0*sum/n;
	printf("With n = %lld tosses, the multi-thread estimate of pi = %f\n", n, sum);

	free(thread_handles);
	return 0;

}  /* main */



// void* get_points(void* rank){

// 	long long int c = 0;
// 	long long int local_n = n/thread_count;

//     srand(time(NULL)+rand()%5);

//     for (int i = 0; i < local_n; i++)
//     {
//         double x_gen = rand();
//         double x = 2*( x_gen/gen_max ) - 1;
//         double y_gen = rand();
//         double y = 2*( y_gen/gen_max ) - 1;
//         double l2 = x*x + y*y;
//         if ( l2 <= 1 )
//         {
//             c++;
//         }
//     }
// 	pthread_mutex_lock(&mutex);
//  	sum += c; 
// 	pthread_mutex_unlock(&mutex);

// 	return NULL;

// }  /* Thread_sum */
void *get_points(void *rank)
{	
	long int num_circle = 0;
	long int local_n;
	double x, y, distance;
	
	local_n = n/thread_count;	

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

