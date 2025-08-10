#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

/* Global variables */

int thread_count;
int m, n;
double* A;
double* x;
double* y;

/* Serial Functions */

void Read_matrix(double A[],int m, int n);
void Read_vector(double x[], int n);

/* Parallel Function */
void *Pth_mat_vect(void* rank);


/*-------------------------------------------------*/
int main(int argc, char* argv[]) {
	
	long thread;
       	pthread_t* thread_handles;
	
	double elapsed=0;
	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));
	
	printf("Enter m and n\n");
	scanf("%d%d", &m, &n);
	
	A = malloc(m*n*sizeof(double));
	x = malloc(n*sizeof(double));
	y = malloc(m*sizeof(double));

	/* serial function to read the matrix A */
	Read_matrix(A,m,n);
		
	/* Serial function to read the matrix x */
	Read_vector(x,n);

	
	struct timespec start,finish;

	clock_gettime(CLOCK_MONOTONIC, &start);	

	for (thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void*) thread);

	for (thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	clock_gettime(CLOCK_MONOTONIC, &finish);	
	elapsed = (finish.tv_sec-start.tv_sec);
	elapsed += (finish.tv_nsec-start.tv_nsec)/1000000000.0;
	
	printf("Your result is: \n");

    int i,j=0;
    for (i = 0; i < m+8*thread_count; i++) {
      printf("%4.1f ", y[i]);
      j++;
      if(j == (m/thread_count)) {
         j = 0;
         i += 8;
      }
    }
	
	printf("\n");
    printf("Elapsed time for private memory implementation %f.\n", elapsed);
	
	free(A);
	free(x);
	free(y);	

	return 0;
}
/*-------------------------------------------------------------*/


void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; 
   int start_row = my_rank*local_m;
   double* private_y = malloc(local_m*sizeof(double));
	
   for (i = 0; i < local_m; i++) {
      private_y[i] = 0.0;
      for (j = 0; j < n; j++)
          private_y[i] += A[(i+start_row)*n+j]*x[j];
   }

   memcpy(y+start_row, private_y, local_m*sizeof(double));
   free(private_y);
   return NULL;
}  /* Pth_mat_vect */

/*-------------------------------------------------------------*/
void Read_matrix(double A[], int m, int n) {

	int i,j;

	printf("Enter serially the elements of matrix A\n");
	for (i=0; i<m; i++)
		for (j=0; j < n; j++)
		       scanf("%lf",&A[i*n+j]);

	printf("You have entered :\n");
	for (i=0; i < m; i++) {
		for (j=0; j < n; j++)
			printf("%4.1f",A[i*n+j]);
		printf("\n");
	}
}	/* Read_matrix */	

/*-----------------------------------------------------------*/
void Read_vector(double x[], int n) {

	int i;

	printf("Enter the elements of vector x\n");
	for (i = 0; i < n; i++)
		scanf("%lf", &x[i]);
	
	printf("You have entered :\n");
	for (i = 0; i < n; i++) 
		printf("%4.1f",x[i]);
	printf("\n");
}  /* Read_vector */

