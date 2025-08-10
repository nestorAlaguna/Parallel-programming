#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>


int main(int argc, char* argv[]) {
   
	int thread_count, n, i, j;
	int count;

   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);

	int *a = malloc(n*sizeof(int));
	int *temp = malloc(n*sizeof(int));
	
	printf("enter array :\n");
	
	for(int k=0; k<n; k++)
	scanf("%d", &a[k]);
	
	/*printf("The entered array is: \n");
	for(int m=0; m<n; m++){
	printf("%d", a[m]);
	}	
	printf("\n");*/
	 
	# pragma omp parallel for num_threads(thread_count)\
	private(i,j,count) shared(a, n, temp)
	for(i=0; i<n; i++)
	{
	count = 0;
	for(j=0; j<n;j++) 
	{
	if(a[j] < a[i]) count ++;
	else if (a[j] == a[i] && j < i)
	count++;
	}
	temp[count] = a[i];
	}
	# pragma omp parallel num_threads(thread_count)\
	shared(a, n, temp)
	{
	int local_n = n/thread_count;
	int index = omp_get_thread_num()*local_n;

	memcpy(&a[index], &temp[index], local_n*(sizeof(int)));
	}

	printf("sorted array : \n");
	for(int k=0; k<n; k++)
	printf("%d ", a[k]);	
	
	printf("\n");

	free (temp);
	return 0;
}	


