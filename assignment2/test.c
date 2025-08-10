#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>
const int MAX_STRING = 100;
void exit_with_error(const char *message)
{
printf("ERROR: %s\n", message);
MPI_Finalize();
exit(1);
}
int main(int argc, char** argv)
{
clock_t start, end;
double start_mpi, end_mpi;
float local_time, local_time_mpi;
float all_time, all_time_mpi;
char pingpong[MAX_STRING];
int my_rank;
int comm_sz;
int src, dest;
MPI_Comm comm;
comm = MPI_COMM_WORLD;
MPI_Init(NULL,NULL);
MPI_Comm_rank(comm, &my_rank);
MPI_Comm_size(comm, &comm_sz);
if(comm_sz!=2)
exit_with_error("Number of process must be 2\n");
MPI_Barrier(comm);
start_mpi = MPI_Wtime();
start = clock();
for (int i=0; i<500; i++)
{
if(i%2==0)
sprintf(pingpong,"Pong");
else
sprintf(pingpong,"Ping");
if (my_rank==(i%2)){
dest = (my_rank+1)%2;
MPI_Send(pingpong,strlen(pingpong)+1,MPI_CHAR,dest,0,comm);
}
else
{
src = i%2;
MPI_Recv(pingpong,MAX_STRING,MPI_CHAR,src,0,comm,MPI_STATUS_IGNORE);
}
}
end = clock();
end_mpi = MPI_Wtime();
local_time_mpi = (float) (end_mpi - start_mpi);
MPI_Reduce(&local_time_mpi,&all_time_mpi,1,MPI_FLOAT,MPI_SUM,0,comm)
;
local_time = (float) (end-start)/CLOCKS_PER_SEC;
MPI_Reduce(&local_time,&all_time,1,MPI_FLOAT,MPI_SUM,0,comm);
if (my_rank==0)
{
printf("Average time using the C built in timer %f\n",
all_time/comm_sz);
printf("Average time using MPI timer %f\n",
all_time_mpi/comm_sz);
}
MPI_Finalize();
return 0;
}
