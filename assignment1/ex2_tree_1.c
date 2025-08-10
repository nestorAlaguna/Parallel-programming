/*Tree structured global sum*/
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <mpi.h>

double f(double x);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);

int main(void) {
    int my_rank, comm_sz, n=1000, local_n;
    double a=0, b=3, h;
    double local_a, local_b, local_int, total_int;
    int core_limit, loop_count, i, j, dest, sender;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    h = (b - a) / n;
    local_n = n / comm_sz; /* number of trapezoids per process */
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    local_int = Trap(local_a, local_b, local_n, h);
    loop_count = (log2(comm_sz));
    total_int = local_int;
    for (i = 1, j = loop_count; i <= loop_count; i++, j--)
{
    core_limit = comm_sz / pow(2, i);
    if (my_rank < core_limit)
{
    sender = my_rank + ceil((comm_sz / pow(2, i)));
    MPI_Recv(&local_int, 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    total_int = total_int + local_int;
}
    if ((my_rank >= core_limit) && (my_rank < pow(2, j)))
{
    dest = my_rank - ceil((comm_sz / pow(2, i)));
    local_int = total_int;
    MPI_Send(&local_int, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
}
}
    if (my_rank == 0) {
    printf("With n = %d trapezoids, our estimate is \n", n);
    fflush(stdout);
    printf("of the integral from %f to %f = %.5f\n", a, b, total_int);
    fflush(stdout);
}
    MPI_Finalize();
return 0;
}
    double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
double estimate, x;
int i;
estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
for (i = 1; i < trap_count; i++) {
x = left_endpt + i * base_len;
estimate += f(x);
}
estimate = estimate * base_len;
return estimate;
}
double f(double x) {
return x * x;
}
