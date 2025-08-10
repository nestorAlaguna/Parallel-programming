#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>

void exit_with_error(const char *message)
{
    printf("ERROR: %s\n", message);
    MPI_Finalize();
    exit(1);
}

void allocate_vec(int **local_vec_p, int vec_size)
{
    *local_vec_p = malloc(vec_size*sizeof(int));
    assert(local_vec_p!=NULL);
}

void gen_vec(int local_vec[], int my_rank, int vec_size, int n)
{
    int gen_max = 100;
    int gen_min = -100;
    srand(time(NULL)+rand()%5+ my_rank);
    int i;

    for (i = 0;i<n;i++)
    {
        local_vec[i] = gen_min + rand() % (gen_max+1 - gen_min);
    }
}
int get_local_max(int local_vec[],int n)
{
    int max = local_vec[0];
    for(int i = 0; i < n; i++)
    {
        if(local_vec[i] > max)
        {
            max = local_vec[i];
        }
    }
    return max;
}

void destroy_vec(int local_vec[])
{
    assert(local_vec!=NULL);
    free(local_vec);
}


int main(int argc, char** argv)
{   
    int *local_vec;
    int my_rank, comm_sz, local_max, global_max, process_max, source, vec_size;
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;
    vec_size = 20;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    allocate_vec(&local_vec, vec_size);

    gen_vec(local_vec, my_rank, vec_size, vec_size);
    local_max = get_local_max(local_vec, vec_size);

    if (my_rank!=0)
    {
        MPI_Send(&local_max,1,MPI_INT,0,0,comm);
    }
    else
    {
        global_max = local_max;
        process_max = 0;
        for (source = 1; source <comm_sz;source++)
        {
            MPI_Recv(&local_max,1,MPI_INT,source,0, comm,MPI_STATUS_IGNORE);
            if(local_max>global_max)
            {
                global_max = local_max;
                process_max = source;
            }
        }
    }

    if (my_rank==0)
    {
        printf("Max is %d belongs to process %d\n", global_max, process_max);
    }

    destroy_vec(local_vec);
    MPI_Finalize();
    return 0;
}


