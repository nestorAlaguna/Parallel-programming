#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>

const int MAX_STRING = 100;
double gen_max = RAND_MAX;


long long int calculate_points(int my_rank, int n)
{
    int counter = 0;
    double x,y,x_gen,y_gen,distance;
    srand(time(NULL)+rand()%5+ my_rank);

    for (int i = 0; i < n; i++)
    {
        x_gen = rand();
        x = 2*( x_gen/gen_max ) - 1;
        y_gen = rand();
        y = 2*( y_gen/gen_max ) - 1;
        distance = x*x + y*y;
        if ( distance <= 1 )
        {
            counter++;
        }
    }
    return counter;
}

void get_input(int my_rank, int comm_sz, int* n_p)
{
    setbuf(stdout,NULL);
    if (my_rank==0)
    {
        printf("Enter number of tosses: \n");
        scanf("%d", n_p);
        if(*n_p<comm_sz)
        {
            exit_with_error("Number of points has to be larger than the number of processes");
        }
    }
    MPI_Bcast(n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}



int main(int argc, char** argv)
{
    int comm_sz, my_rank, n, local_n;
    long long int local_points, global_points;
    double global_est;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    get_input(my_rank, comm_sz, &n);

    local_n = n/comm_sz;

    local_points = calculate_points(my_rank, local_n);

    MPI_Reduce(&local_points,&global_points,1,MPI_LONG_LONG_INT,MPI_SUM,0,MPI_COMM_WORLD);

    global_est = 4*global_points/((double) n);

    if (my_rank ==0)
    {
        printf("the pi estimation is %f \n",n, global_est);
    }

    MPI_Finalize();
    return 0;
}
