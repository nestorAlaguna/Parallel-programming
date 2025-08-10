#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int myMPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root,MPI_Comm comm);


int myMPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root,MPI_Comm comm) {
int my_rank;
int comm_sz;
int i;
MPI_Comm_size(comm, &comm_sz);
MPI_Comm_rank(comm, &my_rank);

if (my_rank == root) {
for (i = 1; i < comm_sz; i++) {
//if (i != root) {
MPI_Send(buffer, count, datatype, i, 0, comm);
//}
}
}
else {
MPI_Recv(buffer, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
}
}

int main(int argc, char* argv[]) {

int my_rank;
int comm_sz;
int local_n;
int rand_vec[3];
int i;
int n = 3;
MPI_Comm comm;
MPI_Init(NULL, NULL);
comm = MPI_COMM_WORLD;
MPI_Comm_size(comm, &comm_sz);
MPI_Comm_rank(comm, &my_rank);
srand(my_rank);

if (my_rank == 0) {
for (i = 0; i < n; i++)
rand_vec[i] = rand() % 5;
printf("The rank %d has the generated vector: [", my_rank);
for (i = 0; i < n; i++)
printf("%d,", rand_vec[i]);
printf("]\n");
myMPI_Bcast(rand_vec, 3, MPI_INT, 0, comm);
}

else {
for (i = 0; i < n; i++) {
myMPI_Bcast(rand_vec, 3, MPI_INT, 0, comm);
printf("The rank %d has received the vector: [", my_rank);
for (i = 0; i < n; i++)
printf("%d,", rand_vec[i]);
printf("]\n");
}
}
MPI_Finalize();
}



