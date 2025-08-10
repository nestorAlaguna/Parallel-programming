#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void generate_matrix(double M[], int m, int n) {
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      M[i * n + j] = random() / ((double)RAND_MAX);
}

void generate_vector(double y[], int n) {
  int i;
  for (i = 0; i < n; i++)
    y[i] = random() / ((double)RAND_MAX);
}

void omp_matrix_vector(double M[], double x[], double y[], int m, int n, int thread_count) {
  int i, j;
  double start, finish, elapsed;
  start = omp_get_wtime();
   //int current_rank;
#pragma omp parallel num_threads(thread_count) \
  default(none) private(i, j) shared(M, x, y, m, n)
{
  int current_rank = omp_get_thread_num();
#pragma omp for
for (i = 0; i < m; i++) {
   y[i + (current_rank * 8)] = 0.0;
   for (j = 0; j < n; j++)
     y[i + (current_rank * 8)] += M[i * n + j] * x[j];
 }
}
   finish = omp_get_wtime();
  elapsed = finish - start;
  printf("Elapsed time: %e seconds.\n", elapsed);
}



void show_matrix(char *title, double M[], int m, int n) {
  int i, j;
  printf("%s\n", title);
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++)
      printf("%4.1f\t", M[i * n + j]);
    printf("\n");
  }
}

void show_vector(char *title, double v[], double m) {
  int i;
  printf("%s\n", title);
  for (i = 0; i < m; i++)
    printf("%4.1f\t", v[i]);
  printf("\n");
}

int main(int argc, char *argv[]) {
  int thread_count;
  int m, n;
  double *M;
  double *x;
  double *y;

  thread_count = strtol(argv[1], NULL, 10);
  m = strtol(argv[2], NULL, 10);
  n = strtol(argv[3], NULL, 10);
  M = malloc(m * n * sizeof(double));
  x = malloc(n * sizeof(double));
  y = malloc((m + 8 * thread_count) * sizeof(double));
  generate_matrix(M, m, n);
  show_matrix("Matrix: ", M, m, n);
  generate_vector(x, n);
  show_vector("Vector: ", x, n);
  omp_matrix_vector(M, x, y, m, n, thread_count);
  {
    int i, j = 0;
    printf("Result:\n");
    for (i = 0; i < m + 8 * thread_count; i++) {
      printf("%f\t", y[i]);
      j++;
      if (j == (m / thread_count)) {
        j = 0;
        i += 8;
      }
    }
    printf("\n");
  }

  free(M);
  free(x);
  free(y);
  return 0;
}
