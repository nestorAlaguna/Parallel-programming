#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

const int MAX_STRING = 100;
double gen_max = RAND_MAX;

void exit_with_error(const char *message)
{   
    printf("ERROR: %s\n", message);
    MPI_Finalize();
    exit(1);
}

void get_input(int my_rank, int comm_sz, int* n_p)
{
    setbuf(stdout,NULL);
    if (my_rank==0)
    {
        printf("Enter number of points to estimate: \n");
        scanf("%d", n_p);
        if(*n_p<comm_sz)
        {
            exit_with_error("Number of points has to be larger than number of processes");
        }    
    }
    MPI_Bcast(n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

int get_points(int my_rank, int n)
{
    int c = 0;

    srand(time(NULL)+rand()%5+ my_rank);

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
    return c;
}

long long int get_global_sum(int local_val, int my_rank, int comm_sz, MPI_Comm comm)
{
    int partner_val;
    int partner_rank;

    for (int reduce_comm_sz=comm_sz; reduce_comm_sz > 1; reduce_comm_sz/=2)
    {
        if (my_rank < reduce_comm_sz)
        {
            if( my_rank >= reduce_comm_sz/2 )
            {
                partner_rank = my_rank - reduce_comm_sz/2;
                MPI_Send(&local_val, 1, MPI_INT, partner_rank, 0, comm);
            }
            else
            {
                partner_rank = my_rank + reduce_comm_sz/2;
                MPI_Recv(&partner_val, 1, MPI_INT, partner_rank, 0, comm, MPI_STATUS_IGNORE);
                local_val += partner_val;
            }
        }
    }
    return local_val;
}

int main()
{
    int comm_sz, my_rank; 
    int local_val, global_sum; 
    long long int local_points, global_points;
    double global_est;
    int n, local_n;
    MPI_Comm comm;
    
    comm = MPI_COMM_WORLD;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    get_input(my_rank, comm_sz, &n);

    local_n = n/comm_sz;

    local_points = get_points(my_rank, local_n);

    local_val = my_rank+1;

    global_points = get_global_sum(local_points,my_rank,comm_sz,comm);

    global_sum = get_global_sum(local_val,my_rank,comm_sz,comm);

    global_est = 4*global_points/((double) n);

    if (my_rank ==0)
    {
        printf("Estimation of pi %f \n", global_est);
    }

    MPI_Finalize();
    return 0;
}