#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>

void exit_with_error(const char *message)
{
    printf("ERROR: %s\n", message);
    exit(1);
}

int main(int argc, char* argv[]) {

    clock_t start, end;   
	int n, count;
	double elapsed;

	n = strtol(argv[1], NULL, 10);

	int *a = malloc(n*sizeof(int));
	int *temp = malloc(n*sizeof(int));

	printf("Enter %d elements array :\n",n);
	
	for(int i=0; i<n; i++)
	{
		scanf("%d", &a[i]);
	}
	
    start = clock();

	for(int i=0; i<n; i++)
	{
		count = 0;
		for(int j=0; j<n;j++) 
		{
			if(a[j] < a[i] || (a[j]==a[i] && j < i)) 
			{
				count ++;
			}
		}
		temp[count] = a[i];
	}

	memcpy(a,temp,n*sizeof(int));

    end = clock();
	elapsed =(float) (end-start)/CLOCKS_PER_SEC;;

	printf("Sorted Version : \n");
	for(int i=0; i<n; i++)
	{
		printf("%d ", a[i]);	
	}

	printf("\n");
	printf("Serial elapsed time = %e seconds\n", elapsed);

	free (temp);
	return 0;
}	


