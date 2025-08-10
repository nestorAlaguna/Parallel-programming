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
    int gen_min = -100;
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

int myMPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int my_rank;
    int comm_sz;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

        if(my_rank==root)
        {
            for (int i=1; i < comm_sz; i++)
            {
            MPI_Send(buffer,count,datatype,i,0,comm);    
            }
        }
        else
        {
            MPI_Recv(buffer,count,datatype,root,0,comm,MPI_STATUS_IGNORE);
        }
    return 0;
}


int main(int argc, char **argv)
{
    int *local_vec;
    int my_rank, comm_sz;
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;
    int root = 0;
    int vec_size = 3;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);

    allocate_vec(&local_vec, vec_size);

    if (my_rank==root)
    {
        gen_vec(local_vec, my_rank, vec_size);
        printf("generated vec is: ");
        
    }
    myMPI_Bcast(local_vec,vec_size,MPI_INT,root, comm);


    if (my_rank!= root)
    {
        printf("My rank is %d my local_vec is: ", my_rank);
        for(int i = 0; i<vec_size;i++)
        {
            printf("  %d  ",local_vec[i]);
        }
    }
    
    printf("\n");

    destroy_vec(local_vec);

    MPI_Finalize();
    return 0;
}
