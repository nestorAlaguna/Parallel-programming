#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int commonSize, localRank;
  int randomNumber[20];
  int localN, *globalN;
  int limit = 20;
  int i;

  MPI_Comm comm;
  MPI_Init(NULL, NULL);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &commonSize);
  MPI_Comm_rank(comm, &localRank);

  srand(localRank);
  for (i = 0; i < limit; i++)
    randomNumber[i] = (rand() % 200) - 99;
  printf("Rank %d numbers are: (", localRank);
  for (i = 0; i < limit; i++)
    printf("%d,", randomNumber[i]);
  printf(")\n");

  localN = randomNumber[0];
  for (i = 1; i < limit; i++)
    if (randomNumber[i] > localN)
      localN = randomNumber[i];
  globalN = (int *)malloc(commonSize * sizeof(int));
  MPI_Gather(&localN, 1, MPI_INT, globalN, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (localRank == 0) {
    int globalAux = globalN[0];
    for (i = 1; i < commonSize; i++)
      if (globalN[i] > globalAux)
        globalAux = globalN[i];
    printf("The biggest integer is: %d.\n", globalAux);
    for (i = 0; i < commonSize; i++) {
      if (globalN[i] == globalAux)
        printf("The rank %d contains the maximum integer.\n", i);
    }
  }

  free(globalN);
  MPI_Finalize();
  return 0;
}
