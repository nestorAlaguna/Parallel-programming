#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void exit_with_error(const char *message)
{
    printf("ERROR: %s\n", message);
    exit(1);
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main(int argc, char* argv[]) 
{
    clock_t start, end;   
	int n, count;
	double elapsed;

	n = 6; 

	int *a = malloc(n*sizeof(int));

	printf("Enter %d elements array :\n",n);

	for(int i=0; i<n; i++)
	{
		scanf("%d", &a[i]);
	}
	
    start = clock();

    qsort(a,n,sizeof(int),compare);
	
    end = clock();
	elapsed =(float) (end-start)/CLOCKS_PER_SEC;;

	printf("Sorted Version : \n");
	for(int i=0; i<n; i++)
	{
		printf("%d ", a[i]);	
	}

	printf("\n");
	printf("Serial elapsed time = %e seconds\n", elapsed);
    return 0;

}