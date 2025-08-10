#include <stdio.h>
#include <stdlib.h>


int main()
{
    int **A;
    int m = 3;
    int n = 4;
    int len =0;
    len = sizeof(int *) * m + sizeof(int) * n * m;
    A = (int **)malloc(len);
  
    // *A = (int *)(A + m);
  
    // for(int i = 0; i < m; i++)
    //     A[i] = (*A + n * i);

	printf("Enter serially the elements of matrix A\n");
    for(int i = 0; i < m; i++)
    {
        for(int j=0; j<n; j++){
            scanf("%d",&A[i][j]);
        }
    }

    for(int i = 0; i < 3; i++)
    {
        for(int j=0; j<4; j++){
            printf(" %d ", A[i][j]);
        }
        printf("\n");
    }
    // free(A);    
    return 0;
}