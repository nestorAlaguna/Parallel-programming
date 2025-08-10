#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void rand_generator(int n, int rnd_nums[], int my_rank);

int main(int argc, char* argv[]){

int comm_sz, my_rank;
int * local_rnd_nums;
int local_m, * global_m;
int i;
MPI_Comm comm;
MPI_Init(NULL, NULL);
comm = MPI_COMM_WORLD;
MPI_Comm_size(comm, &comm_sz);
MPI_Comm_rank(comm, &my_rank);
v
// Store n random integers
int n = 20;
srand(my_rank);
rand_generator(n, local_rnd_nums, my_rank);


// Calculate local maximum
local_m = local_rnd_nums[0];
for (i = 1; i < n; i++)
if (local_rnd_nums[i] > local_m)
local_m = local_rnd_nums[i];

// Extract the local maximum from each process
global_m = (int*)malloc(comm_sz * sizeof(int));
MPI_Gather(&local_m, 1, MPI_INT, global_m, 1, MPI_INT,0, MPI_COMM_WORLD);
if (my_rank == 0){
int global_l = global_m[0];
for (i = 1; i < comm_sz; i++)
if (global_m[i] > global_l)
global_l = global_m[i];


// Print the biggest integer and the rank that contains it
printf("The biggest integer  is: %d\n", global_l);

for (i = 0; i < comm_sz; i++) {
if (global_m[i] == global_l)
printf("Rank %d contains the maximum\n", i);
}
}

free(global_m);
MPI_Finalize();
return 0;
}

void rand_generator(int n, int rnd_nums[], int my_rank)
{
	int i,t;
	int min = -100;
	for (i = 0; i < n; i++)
		rnd_nums[i]= (rand()% 199) +  min;
    printf("For rank %d the random numbers are: [", my_rank);
    for (t = 0; t < n; t++)
    printf("%d,", rnd_nums[t]);
    printf("]\n");
}


