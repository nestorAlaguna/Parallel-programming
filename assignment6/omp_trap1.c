#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
void Usage(char* prog_name);
double f(double x); /* Function we're integrating */
double Trap(double a, double b, int n, int thread_count);
int main(int argc, char* argv[]) {

    double global_result = 0.0; /* Store result in global_result */
    double a, b; /* Left and right endpoints */
    int n; /* Total number of trapezoids */
    int thread_count;
    if (argc != 2) Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);
    global_result = Trap(a, b, n, thread_count);
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n",
    a, b, global_result);
    return 0;
} /* main */
/*--------------------------------------------------------------------*/
double f(double x) {
    double return_val;
    return_val = x*x;
    return return_val;
}
/*------------------------------------------------------------------*/
double Trap(double a, double b, int n, int thread_count) {
    double h, approx;
    int i, thread_nu;
    int* it = malloc(n*sizeof(int));
    h = (b-a)/n;
    approx = (f(a) + f(b))/2.0;
    # pragma omp parallel for num_threads(thread_count) \

    reduction(+: approx) schedule(runtime)
    for (i = 1; i <= n-1; i++){
        approx += f(a + i*h);
        it[i] = omp_get_thread_num(); /*array of the number of iterations*/
    }
    approx = h*approx;
    printf(" Iteration 1 and thread %d ", it[0]);
    thread_nu = it[0];
    for (i = 2; i < n; i++)
        if (thread_nu != it[i]) {
        printf("\n");
            printf("Iteration %d and thread %d ", i, it[i]);
    }
    if (thread_nu == it[n-1]) {
        printf("Iteration%d and thread %d ", n, it[n-1]);
    } else {
        printf("\n");
        printf(" Iteration %d and thread %d ", n, it[n-1]);
    }
    printf("\n");


    free(it);
        return approx;
}
