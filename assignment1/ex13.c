#include <stdio.h>
#include <mpi.h>
#include<time.h>

int main(void) {

int ping = 1;
int pong = 2;
int count = 500;
int comm_sz, my_rank;
double str, fin, loc_elap, elap;
double str_c, fin_c, loc_elap_c, elap_c;
str = MPI_Wtime();
str_c = (double)clock();


MPI_Comm comm;
MPI_Init(NULL, NULL);
comm = MPI_COMM_WORLD;
MPI_Comm_size(comm, &comm_sz);
MPI_Comm_rank(comm, &my_rank);
MPI_Barrier(comm);


for (int i = 0; i < count; i++) {

if (my_rank == 1) {
MPI_Send(&pong, 1, MPI_INT, 0, 0, comm);
MPI_Recv(&ping, 1, MPI_INT, 0, 0, comm, MPI_STATUSES_IGNORE);
}
if (my_rank == 0) {
MPI_Send(&ping, 1, MPI_INT, 1, 0, comm);
MPI_Recv(&pong, 1, MPI_INT, 1, 0, comm, MPI_STATUSES_IGNORE);
}

}

fin = MPI_Wtime();
fin_c = (double)clock();
loc_elap = fin - str;
loc_elap_c = (fin_c - str_c)/ (double)CLOCKS_PER_SEC;
MPI_Reduce(&loc_elap, &elap, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
MPI_Reduce(&loc_elap_c, &elap_c, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
loc_elap_c = (fin_c - str_c)/ (double)CLOCKS_PER_SEC;

if (my_rank == 0) {
printf("Elapsed time with MPI_Wtime is: %f\n", elap);
printf("Elapsed time with C clock function is: %f\n", elap_c);
}

MPI_Finalize();
return 0;

}
