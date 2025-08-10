#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void rand_generator(int n, int rnd_array[], int my_rank);
int main(int argc, char* argv[])
{
int comm_sz, my_rank;
int i;int n = 20;
int rnd_array[20];
int local_m, * global_m;
MPI_Comm comm;
MPI_Init(NULL, NULL);
comm = MPI_COMM_WORLD;
MPI_Comm_size(comm, &comm_sz);
MPI_Comm_rank(comm, &my_rank);


// Create n random integers

srand(my_rank);
rand_generator(n, rnd_array, my_rank);

// Calculate local maximum

local_m = rnd_array[0];
for (i = 1; i < n; i++)
if (rnd_array[i] > local_m)
local_m = rnd_array[i];

// Extract local maximum from each process

global_m = (int*)malloc(comm_sz * sizeof(int));
MPI_Gather(&local_m, 1, MPI_INT, global_m, 1, MPI_INT,
0, MPI_COMM_WORLD);
if (my_rank == 0)
{
int global_l = global_m[0];
for (i = 1; i < comm_sz; i++)
if (global_m[i] > global_l)
global_l = global_m[i];
// Print
printf("The biggest integer is: %d\n", global_l);
for (i = 0; i < comm_sz; i++) {
if (global_m[i] == global_l)
printf("Rank %d has the maximum\n", i);
}
}
free(global_m);
MPI_Finalize();
return 0;
}


void rand_generator(int n, int rnd_array[], int my_rank)
{
    int i,t;
	for (i = 0; i < n; i++)
rnd_array[i] = (rand() % 200) - 100;
printf("For rank %d numbers are: [", my_rank);
for (i = 0; i < n; i++)
printf("%d,", rnd_array[i]);
printf("]\n");
}
