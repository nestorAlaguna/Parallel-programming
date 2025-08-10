#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>
#include <math.h>

void exit_with_error(const char *message)
{
    printf("ERROR: %s\n", message);
    MPI_Finalize();
    exit(1);
}

void allocate_local_mat(int **local_mat_p, int local_mat_size)
{
    *local_mat_p = malloc(local_mat_size*sizeof(int));

    if(*local_mat_p==NULL)
    {
        exit_with_error("Cannot allocate memory for matrix");
    }
}

void gen_local_mat(int local_mat[], int my_rank, int local_n)
{
    for (int i = 0;i<local_n;i++)
    {
        for (int j=0; j<local_n;j++)
        {
            local_mat[i*local_n+j] = (i+j+1)*(my_rank);
        }
    }
}

void set_mat_zeros(int partner_mat[], int local_n)
{
    for (int i = 0;i<local_n;i++)
    {
        for (int j=0; j<local_n;j++)
        {
            partner_mat[i*local_n+j] = 0;
        }
    }
}


void print_mat(int local_mat[], int local_n)
{
    for(int i = 0; i < local_n; i++)
    {
        for(int j = 0; j < local_n; j++)
        {
            printf(" %d ", local_mat[i*local_n+j]);
        }
        printf("\n");
    }
}


void print_partner_mat(int local_mat[], int local_n)
{
    // for(int i = 0; i < local_n; i++)
    // {
        for(int j = 0; j < local_n; j++)
        {
            printf(" %d ", local_mat[j]);
        }
        printf("\n");
    // }
}



void sum_mat(int local_mat[], int partner_mat[], int local_n, int sign, int offset)
{
    if(sign==1)
    {  
        for(int i = 0; i < local_n; i++)
        {
            local_mat[i + offset]=  local_mat[i + offset] + partner_mat[i];
        }
    }
    else
    {
        for(int i = 0; i < local_n; i++)
        {
            local_mat[i + offset]=   partner_mat[i ] - local_mat[i+ offset];
        }
    }
}


void destroy_mat(int local_vec[])
{
    assert(local_vec!=NULL);
    free(local_vec);
}

int main(int agrc, char **argv)
{
    int *local_mat, *partner_mat;

    int my_rank, partner_rank, comm_sz, local_mat_size, partner_size, n, local_n;

    MPI_Comm comm;
    MPI_Status *ignore_status;
    ignore_status = MPI_STATUS_IGNORE;
    comm = MPI_COMM_WORLD;
    n = 4;
    local_n = (int) n/2;

    local_mat_size = (int) (local_n)*(local_n);
    partner_size = local_n;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &comm_sz);
    
    allocate_local_mat(&local_mat, local_mat_size);
    allocate_local_mat(&partner_mat, partner_size);


    gen_local_mat(local_mat,my_rank, local_n);
 
    for (int i = 0; i<comm_sz/2;i++)
    {
        if(my_rank%2==i)
        {
            partner_rank = my_rank - (i*2 - 1);
        }
        else if (my_rank%2!=i)
        {
            partner_rank = my_rank + (i*2 - 1);

        }
    }

    for (int offset = 0; offset <=2 ; offset+=2)
    {
        MPI_Send((local_mat+offset), partner_size,MPI_INT, partner_rank, 0,comm);
        MPI_Recv(partner_mat, partner_size, MPI_INT, partner_rank,0,comm,ignore_status);
        if (my_rank%2==0)
        {
            sum_mat(local_mat,partner_mat, local_n, 1,offset);
        }
        else
        {
            sum_mat(local_mat,partner_mat, local_n,0,offset);
        }

    }

    for(int reduce_coeff = 2; reduce_coeff <= comm_sz; reduce_coeff*=2)
    {
        if(my_rank% reduce_coeff < reduce_coeff/2)
        {
            partner_rank = my_rank + reduce_coeff/2;

        }
        else
        {
            partner_rank = my_rank - reduce_coeff/2;
        }
    }
    for (int offset = 0; offset <=2 ; offset+=2)
    {
        MPI_Send((local_mat+offset), partner_size,MPI_INT, partner_rank, 0,comm);
        MPI_Recv(partner_mat, partner_size, MPI_INT, partner_rank,0,comm,ignore_status);
        if (my_rank<2)
        {
            sum_mat(local_mat,partner_mat, local_n, 1,offset);
        }
        else
        {
            sum_mat(local_mat,partner_mat, local_n,0,offset);
        }

    }

    for (int i = 0; i < 4; i++)
    {
        if(my_rank==i)
        {
            printf("my rank is %d\n", my_rank);
            print_mat(local_mat,local_n);
        }
    }

    destroy_mat(local_mat);
    destroy_mat(partner_mat);

    MPI_Finalize();
    return 0;
}

