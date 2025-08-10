#include <assert.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void ExitWithError(const char *message) {
  printf("ERROR: %s\n", message);
  MPI_Finalize();
  exit(1);
}

void AllocateLocalMatrix(int **localMatrixP, int localMatrixSize) {
  *localMatrixP = malloc(localMatrixSize * sizeof(int));
  if (*localMatrixP == NULL) {
    ExitWithError("Cannot allocate memory for matrix!");
  }
}

void GenerteLocalMatrix(int localMatrix[], int matrixRank, int localN) {
  for (int i = 0; i < localN; i++) {
    for (int j = 0; j < localN; j++) {
      localMatrix[i * localN + j] = (i + j + 1) * (matrixRank);
    }
  }
}

void SetMatrixZero(int partnerMatrix[], int localN) {
  for (int i = 0; i < localN; i++) {
    for (int j = 0; j < localN; j++) {
      partnerMatrix[i * localN + j] = 0;
    }
  }
}

void PrintMatrix(int localMatrix[], int localN) {
  for (int i = 0; i < localN; i++) {
    for (int j = 0; j < localN; j++) {
      printf("\t%d", localMatrix[i * localN + j]);
    }
    printf("\n");
  }
}

void AddMatrix(int localMatrix[], int partnerMatrix[], int localN, int sign) {
  if (sign == 1) {
    for (int i = 0; i < localN; i++) {
      for (int j = 0; j < localN; j++) {
        localMatrix[i * localN + j] =
            localMatrix[i * localN + j] + partnerMatrix[i * localN + j];
      }
    }
  } else {
    for (int i = 0; i < localN; i++) {
      for (int j = 0; j < localN; j++) {
        localMatrix[i * localN + j] =
            partnerMatrix[i * localN + j] - localMatrix[i * localN + j];
      }
    }
  }
}

void DestroyMatrix(int localMatrix[]) {
  assert(localMatrix != NULL);
  free(localMatrix);
}

int main(int agrc, char **argv) {
  int *localMatrix, *partnerMatrix;
  int matrixRank, partnerRank, commonSize, localMatrixSize, n, localN;
  MPI_Comm comm;
  MPI_Status *ignoreStatus;
  ignoreStatus = MPI_STATUS_IGNORE;
  MPI_Status recoveryStatus;
  comm = MPI_COMM_WORLD;
  n = 4;
  localN = (int)n / 2;
  MPI_Request request;
  localMatrixSize = (int)(localN) * (localN);

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(comm, &matrixRank);
  MPI_Comm_size(comm, &commonSize);

  AllocateLocalMatrix(&localMatrix, localMatrixSize);
  AllocateLocalMatrix(&partnerMatrix, localMatrixSize);
  GenerteLocalMatrix(localMatrix, matrixRank, localN);

  for (int i = 0; i < commonSize / 2; i++) {
    if (matrixRank % 2 == i) {
      partnerRank = matrixRank - (i * 2 - 1);
      MPI_Isend(localMatrix, localMatrixSize, MPI_INT, partnerRank, 0, comm,
                &request);
    } else {
      partnerRank = matrixRank + (i * 2 - 1);
      MPI_Recv(partnerMatrix, localMatrixSize, MPI_INT, partnerRank, 0, comm,
               &recoveryStatus);
    }
  }

  MPI_Wait(&request, &recoveryStatus);

  if (matrixRank % 2 == 0) {
    AddMatrix(localMatrix, partnerMatrix, localN, 1);
  } else {
    AddMatrix(localMatrix, partnerMatrix, localN, 0);
  }
  for (int reduceCoeff = 2; reduceCoeff <= commonSize; reduceCoeff *= 2) {
    if (matrixRank % reduceCoeff < reduceCoeff / 2) {
      partnerRank = matrixRank + reduceCoeff / 2;
      MPI_Isend(localMatrix, localMatrixSize, MPI_INT, partnerRank, 0, comm,
                &request);
    } else {
      partnerRank = matrixRank - reduceCoeff / 2;
      MPI_Recv(partnerMatrix, localMatrixSize, MPI_INT, partnerRank, 0, comm,
               &recoveryStatus);
    }
  }
  MPI_Wait(&request, &recoveryStatus);

  if (matrixRank < 2) {
    AddMatrix(localMatrix, partnerMatrix, localN, 1);
  } else {
    AddMatrix(localMatrix, partnerMatrix, localN, 0);
  }

  for (int i = 0; i < 4; i++) {
    if (matrixRank == i) {
      printf("Matrix rank is %d.\n", matrixRank);
      PrintMatrix(localMatrix, localN);
    }
  }
  DestroyMatrix(localMatrix);
  DestroyMatrix(partnerMatrix);
  MPI_Finalize();
  return 0;
}