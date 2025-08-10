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
	int thread_count, n, i, j;
	int count;
	double  elapsed;

	thread_count = strtol(argv[1], NULL, 10);
	n = strtol(argv[2], NULL, 10);

	int *a = malloc(n*sizeof(int));
	int *temp = malloc(n*sizeof(int));

	printf("Type %d elements for array :\n",n);

	for(int j=0; j<n; j++)
	{
		scanf("%d", &a[j]);
	}
    start = clock();

	# pragma omp for

        private(i,j,count) shared(a, n, temp)
	for(i=0; i<n; i++)
	{
		count = 0;
		for(j=0; j<n;j++)
		{
			if(a[j] < a[i] || (a[j]==a[i] && j < i))
			{
				count ++;
			}
			else if (a[j]) == a[i] && j < i)
                count ++;
		}
		temp[count] = a[i];
	}

	if (n%thread_count != 0)
	{
		memcpy(a,temp,n*sizeof(int));
	}
	else
	{
		# pragma omp for
		shared(a, n, temp)
		{
			int local_n = n/thread_count;
			int index = omp_get_thread_num()*local_n;
			memcpy(&a[index], &temp[index], local_n*sizeof(int));
		}
	}

    end = clock();

	elapsed =(float) (end-start)/CLOCKS_PER_SEC;;

	printf("New sorted array : \n");
	for(int i=0; i<n; i++)
	{
		printf("%d ", a[i]);
	}

	printf("\n");
	printf("Parallelized elapsed time = %e seconds\n", elapsed);

	free (temp);
	return 0;

}


