#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void Input(long long int* num_p, int my_rank);
long long int Calculation(double local_n, long long int* num_p);
void Output(int my_rank, double pi, long long int total_sum, long long int* num_p);


void Input(long long int* num_p, int my_rank){

if (my_rank == 0){
printf("Please specify number of tosses \n");
fflush(stdout);
scanf("%llu", num_p);
fflush(stdout);
}

MPI_Bcast(num_p, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
}

long long int num_in_circle = 0;
long long int Calculation(double local_n, long long int* num_p){
double x, y, distance;

srand(time(NULL));
for (int i = 0; i < local_n; i++){
x = (double)rand() / RAND_MAX * 2.0 - 1.0;
y = (double)rand() / RAND_MAX * 2.0 - 1.0;
distance = x * x + y * y;

if (distance <= 1){ 
num_in_circle++;
}
}
return num_in_circle;
}


void Output(int my_rank, double pi, long long int total_sum, long long int* num_p){

if (my_rank == 0){
pi = 4 * total_sum / ((double)*num_p);
printf("Estimated value of pi = %.15f \n", pi);
}
}


int main(void){

int comm_sz, my_rank;
double pi = 0;
long long int local_n, local_result, total_sum;
long long int* num_p = NULL, n;
num_p = &n;

MPI_Init(NULL, NULL);
MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
Input(num_p, my_rank);
local_n = n / comm_sz;
local_result = Calculation(local_n, num_p);
MPI_Reduce(&local_result, &total_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

if (my_rank == 0){
Output(my_rank, pi, total_sum, num_p);
}

MPI_Finalize();
return 0;
}
