/* File:     
 *     omp_mat_vect.c 
 *
 *
 * Purpose:  
 *     Computes a parallel matrix-vector product.  Matrix
 *     is distributed by block rows.  Vectors are distributed by 
 *     blocks.  Unless the DEBUG flag is turned on this version 
 *     uses a random number generator to generate A and x.
 *
 *
 * Compile:  
 *    gcc -g -Wall -fopenmp -o omp_mat_vect omp_mat_vect.c 
 * Usage:
 *    omp_mat_vect <thread_count> <m> <n>
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Serial functions */

void Gen_matrix(double A[], int m, int n);
void Gen_vector(double x[], int n);
void Print_matrix(char* title, double A[], int m, int n);
void Print_vector(char* title, double y[], double m);

/* Parallel function */
void Omp_mat_vect(double A[], double x[], double y[],
      int m, int n, int thread_count);

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int     thread_count;
   int     m, n;
   double* A;
   double* x;
   double* y;


   /* Get Arguments */
   thread_count = strtol(argv[1], NULL, 10);
   m = strtol(argv[2], NULL, 10);
   n = strtol(argv[3], NULL, 10);


   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
   Gen_matrix(A, m, n);
   Print_matrix("We generated", A, m, n); 

   Gen_vector(x, n);
   Print_vector("We generated", x, n); 

   Omp_mat_vect(A, x, y, m, n, thread_count);
 
   Print_vector("The product is", y, m); 

   free(A);
   free(x);
   free(y);

   return 0;
}  /* main */


/*----------------------------------------*/
void Gen_matrix(double A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = random()/((double) RAND_MAX);
}  /* Gen_matrix */

/*-----------------------------------*/
void Gen_vector(double x[], int n) {
   int i;
   for (i = 0; i < n; i++)
      x[i] = random()/((double) RAND_MAX);
}  /* Gen_vector */


/*------------------------------------------------------------------
 * Function:  Omp_mat_vect
 * Purpose:   Multiply an mxn matrix by an nx1 column vector
 * In args:   A, x, m, n, thread_count
 * Out arg:   y
 */
void Omp_mat_vect(double A[], double x[], double y[],
      int m, int n, int thread_count) {
   int i, j, l;
   double start, finish, elapsed;
   int local_m = m/thread_count;
   double* private_y;

   start = omp_get_wtime();
#  pragma omp parallel num_threads(thread_count) \
      default(none) private(i, j, l, private_y) \
      shared(A, x, y, m, n, local_m)
   {   
      l = 0;
      private_y = malloc(local_m*sizeof(double));
#     pragma omp for
      for (i = 0; i < m; i++) {
         private_y[l] = 0.0;
         for (j = 0; j < n; j++)
            private_y[l] += A[i*n+j]*x[j];
         l++;
      }
      
      l = 0;
#     pragma omp for
      for (i = 0; i < m; i++)
         y[i] = private_y[l++];

      free(private_y);
   }  /* pragma omp parallel */

   finish = omp_get_wtime();
   elapsed = finish - start;
   printf("Elapsed time = %e seconds\n", elapsed);
   
}  /* Omp_mat_vect */


/*---------------------------------------------------*/
void Print_matrix( char* title, double A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
}  /* Print_matrix */


/*-------------------------------------*/
void Print_vector(char* title, double y[], double m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}  /* Print_vector */
