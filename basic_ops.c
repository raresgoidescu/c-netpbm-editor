#include <stdio.h>

void my_swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}
