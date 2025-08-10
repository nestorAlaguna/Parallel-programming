#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>


int myMPI_Reduce(int* sendbuf, int* recvbuf, int count, MPI_Datatype datatype, int root, MPI_Comm comm){

int my_rank, comm_sz;
MPI_Comm_rank(comm, &my_rank);
MPI_Comm_size(comm, &comm_sz);

if (my_rank != root){
MPI_Send(sendbuf, count, datatype, root, 0, comm);
}
else{
for (int i = 1; i < comm_sz; i++)
{
MPI_Recv(recvbuf, count, datatype, i, 0, comm, MPI_STATUS_IGNORE);
if (*recvbuf < *sendbuf){
*sendbuf = *recvbuf;
}
}
}
return 0;
}


int main(int argc, char** argv){

int sendbuf, recvbuf;
int my_rank, comm_sz;

MPI_Comm comm;
comm = MPI_COMM_WORLD;
MPI_Init(NULL, NULL);
MPI_Comm_rank(comm, &my_rank);
MPI_Comm_size(comm, &comm_sz);
srand(my_rank + 1 + time(NULL));
sendbuf = rand() % 100;

printf("The processor %d has the number %d\n", my_rank, sendbuf);
myMPI_Reduce(&sendbuf, &recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

if (my_rank == 0)
{
printf("The minimum of all of them %d", sendbuf);
}
printf("\n");
MPI_Finalize();
return 0;
}

