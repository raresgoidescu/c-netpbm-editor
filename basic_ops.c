// Copyright Rares-Stefan Goidescu 312CAb 2023-2024

#include <stdio.h>

/* Interschimba doua variabile de tip int */
void my_swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}
