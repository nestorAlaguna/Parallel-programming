#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

double gen_max = RAND_MAX;


void exit_with_error(const char *message)
{
    printf("ERROR: %s\n", message);
    exit(1);
}

int main(int argc, char *argv[])
{
	long thread_count; 
 	long long int n, c=0;
 	double pi_estimate;
	
	thread_count = strtol(argv[1],NULL,10);
	n  =  strtol(argv[2], NULL, 10); 
	# pragma omp parallel for num_threads(thread_count) \
     	 reduction(+: c)
	
    for (int i = 0; i < n; i++)
    {
        double x_gen = rand();
        double x = 2*( x_gen/gen_max ) - 1;
        double y_gen = rand();
        double y = 2*( y_gen/gen_max ) - 1;
        double l2 = x*x + y*y;
        if ( l2 <= 1 )
        {
            c++;
        }
    }

	pi_estimate = 4.0*c/n;

	printf("With %lld tosses the pi estimation is %f\n",n,pi_estimate);
	
	return 0;

} 

	
