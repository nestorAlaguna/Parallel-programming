#include <stdio.h>
#include <stdlib.h>
int main()
{
    int *a;
    a = malloc(4*sizeof(int));
    a[0]=0;
    a[1]=1;
    a[2]=2;
    a[3]=3;
    if (sizeof(a)==4*sizeof(int))
    {
        printf("yay\n");
    }
    else
    {

        printf("%d\n",*(a+1));
    }
    free(a);
    return 0;
}