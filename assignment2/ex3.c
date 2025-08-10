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
}

void gen_vec(int local_vec[], int my_rank, int vec_size)
{
    int gen_max = 100;
    int gen_min = 1;
    srand(time(NULL)+rand()%5+ my_rank);
    int i;
    for (i = 0;i<vec_size;i++)
    {
        local_vec[i] = gen_min + rand() % (gen_max+1 - gen_min);
    }
}

void destroy_vec(int local_vec[])
{
    assert(local_vec!=NULL);
    free(local_vec);
}

int myMPI_Reduce(int *local, int *partner, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int my_rank, comm_sz;

    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

        if(my_rank!=root)
        {
            MPI_Send(local,count,datatype,root,0,comm);    
        }
        else
        {
            for (int i=1; i < comm_sz; i++)
            {
            MPI_Recv(partner,count,datatype,i,0,comm,MPI_STATUS_IGNORE);
            if(*partner<*local)
            {
                *local = *partner;
            }
            }
        }
    return 0;
}


int main(int argc, char **argv)
{
    int local, partner;
    int my_rank, comm_sz;
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;
    int root = 0;
    int vec_size = 1;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    srand(time(NULL)+rand()%5+ my_rank);

    int gen_max = 100;
    int gen_min = 1;

    local = gen_min + rand() % (gen_max+1 - gen_min);

    printf("my rank is %d local %d\n", my_rank, local);
    myMPI_Reduce(&local,&partner,vec_size,MPI_INT,root, MPI_COMM_WORLD);

    if (my_rank== root)
    {
        printf("the global minimum is %d", local);
    }
    
    printf("\n");

    MPI_Finalize();
    return 0;
}
