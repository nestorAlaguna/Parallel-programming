#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm);
void get_input(int my_rank, int comm_sz, int* n_p);

void get_input(int my_rank, int comm_sz, int* n_p) {

    setbuf(stdout,NULL);

    if (my_rank==0) {
        printf("Enter maximum of numbers to sum \n");
        scanf("%d", n_p);

    }

    MPI_Bcast(n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm) {

   int my_sum = my_int;
   int partner, recvtemp;
   unsigned bitmask = 1;

   while (bitmask < comm_sz) {
      partner = my_rank ^ bitmask;

      MPI_Sendrecv(&my_sum, 1, MPI_INT, partner, 0,
            &recvtemp, 1, MPI_INT, partner, 0, comm,
            MPI_STATUS_IGNORE);
      my_sum += recvtemp;
      bitmask <<= 1;
   }

   return my_sum;
}

int MAX_CONTRIB = 0;

int main(void) {
   int i, sum, my_int,n;
   int my_rank, comm_sz;
   int* all_integers = NULL;
   MPI_Comm comm;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   get_input(my_rank, comm_sz, &n);
   MAX_CONTRIB = n;
   srandom(my_rank + 1);
   my_int = random() % MAX_CONTRIB;

   sum = Global_sum(my_int, my_rank, comm_sz, comm);

   if ( my_rank == 0) {
      all_integers = malloc(comm_sz*sizeof(int));
      MPI_Gather(&my_int, 1, MPI_INT, all_integers, 1, MPI_INT, 0, comm);
      printf("Integers being summed:\n   ");
      for (i = 0; i < comm_sz; i++)
        printf("%d ", all_integers[i]);
      printf("\n");
      MPI_Gather(&sum, 1, MPI_INT, all_integers, 1, MPI_INT, 0, comm);
      printf("The sums are = \n   ");
      for (i = 0; i < comm_sz; i++)
        printf("%d ", all_integers[i]);
      printf("\n");
      free(all_integers);
   } else {
      MPI_Gather(&my_int, 1, MPI_INT, all_integers, 1, MPI_INT, 0, comm);
      MPI_Gather(&sum, 1, MPI_INT, all_integers, 1, MPI_INT, 0, comm);
   }

   MPI_Finalize();
   return 0;
}


