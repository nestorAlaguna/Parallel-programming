#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
void Generate_matrix(double local_A[], int my_rank, int local_n);
void Allocate_mat(double** local_A_pp, int local_m,MPI_Comm comm);
void partner_mat(int partner_mat[], int local_n);
void sum(int local_A[], int partner_mat[], int local_n, int n, int m);

int main(int agrc, char** argv){
int* local_A, * partner_mat;
int my_rank, partner_rank, comm_sz, local_m, n, local_n, m;
MPI_Comm comm;
comm = MPI_COMM_WORLD;
n = 4;
local_n = (int)n / 2;
local_m = (int)(local_n) * (local_n);
MPI_Init(NULL, NULL);
MPI_Comm_rank(comm, &my_rank);
MPI_Comm_size(comm, &comm_sz);
Allocate_mat(&local_A, local_m, comm);
Allocate_mat(&partner_mat, local_m, comm);
srand(my_rank);
Generate_matrix(local_A, my_rank, local_n);
for (int i = 0; i < comm_sz / 2; i++)
if (my_rank % 2 == i)
partner_rank = my_rank - (i * 2 - 1);
else
partner_rank = my_rank + (i * 2 - 1);
for (int m = 0; m <= 2; m += 2) {
MPI_Send(local_A, local_m, MPI_INT, partner_rank, 0, comm);
MPI_Recv(partner_mat, local_m, MPI_INT, partner_rank, 0, comm, MPI_STATUS_IGNORE);
if (my_rank % 2 == 0)
sum(local_A, partner_mat, local_n, 1, m);
else
sum(local_A, partner_mat, local_n, 0, m);
}
for (int reduce_coeff = 2; reduce_coeff <= comm_sz; reduce_coeff *= 2)
if (my_rank % reduce_coeff < reduce_coeff / 2)
partner_rank = my_rank + reduce_coeff / 2;
else
partner_rank = my_rank - reduce_coeff / 2;
for (int m = 0; m <= 2; m += 2) {
MPI_Send(local_A, local_m, MPI_INT, partner_rank, 0, comm);
MPI_Recv(partner_mat, local_m, MPI_INT, partner_rank, 0, comm, MPI_STATUS_IGNORE);
if (my_rank < 2) {
sum(local_A, partner_mat, local_n, 1, m);
}
else
{
sum(local_A, partner_mat, local_n, 0, m);
}
}
if (my_rank == 0) {
for (int i = 0; i < local_n; i++)
{
for (int j = 0; j < local_n; j++)
{
printf(" %d\t ", local_A[i * local_n + j]);
}
printf("\n");
}
}
else if (my_rank == 1)
for (int i = 0; i < local_n; i++)
{
for (int j = 0; j < local_n; j++)
{
printf("\t\t%d", local_A[i * local_n + j]);
}
printf("\n");
}
else if (my_rank == 2)
for (int i = 0; i < local_n; i++)
{
for (int j = 0; j < local_n; j++)
{
printf(" %d\t", local_A[i * local_n + j]);
}
printf("\n");
}
else if (my_rank == 3)
for (int i = 0; i < local_n; i++)
{
for (int j = 0; j < local_n; j++)
{
printf("\t\t%d", local_A[i * local_n + j]);
}
printf("\n");
}
free(local_A);
free(partner_mat);
MPI_Finalize();
return 0;
}

void Allocate_mat(double** local_A_pp,int local_m,MPI_Comm comm) {
*local_A_pp = malloc(local_m* sizeof(int));
}

void Generate_matrix(double local_A[],int my_rank,int local_n) {
int i, j;
for (i = 0; i < local_n; i++)
for (j = 0; j < local_n; j++)
local_A[i * local_n + j] = (i + j + 1) * (my_rank);
}

void partner_mat(int partner_mat[], int local_n)
{
for (int i = 0; i < local_n; i++)
for (int j = 0; j < local_n; j++)
partner_mat[i * local_n + j] = 0;
}
void sum(int local_A[], int partner_mat[], int local_n, int n, int m)
{
if (n == 1)
for (int i = 0; i < local_n; i++)
local_A[i + m] = local_A[i + m] + partner_mat[i];
else
for (int i = 0; i < local_n; i++)
local_A[i + m] = partner_mat[i] - local_A[i + m];
}
