// Copyright Rares-Stefan Goidescu 312CAb 2023-2024

#include <stdio.h>
#include <stdlib.h>

/******************************PRIVATE FUNCTIONS******************************/

void handle_null_pointer(void *p)
{
	if (!p) {
		printf("Eroare la alocare\n");
		exit(-1);
	}
}

/******************************EXPORTED FUNCTIONS*****************************/

/*
	Aceste functii aloca si elibereaza memorie dinamic.
	Nu sunt sigur daca pot face o functie care sa poata aloca o matrice
	cu elemente de orice tip (int/double/unsigned)
*/

unsigned int **allocate_matrix(int n, int m)
{
	unsigned int **ptr = NULL;
	ptr = malloc(n * sizeof(unsigned int *));
	handle_null_pointer(ptr);

	for (int i = 0; i < n; ++i) {
		ptr[i] = malloc(m * sizeof(unsigned int));
		handle_null_pointer(ptr[i]);
		if (!ptr) {
			for (int j = 0; j < i; j++)
				free(ptr[j]);
			free(ptr);
		}
	}

	return ptr;
}

int **allocate_int_matrix(int n, int m)
{
	int **ptr = NULL;
	ptr = malloc(n * sizeof(int *));
	handle_null_pointer(ptr);

	for (int i = 0; i < n; ++i) {
		ptr[i] = malloc(m * sizeof(int));
		handle_null_pointer(ptr[i]);
		if (!ptr) {
			for (int j = 0; j < i; j++)
				free(ptr[j]);
			free(ptr);
		}
	}

	return ptr;
}

double **allocate_double_matrix(int n, int m)
{
	double **ptr = NULL;
	ptr = malloc(n * sizeof(double *));
	handle_null_pointer(ptr);

	for (int i = 0; i < n; ++i) {
		ptr[i] = malloc(m * sizeof(double));
		handle_null_pointer(ptr[i]);
		if (!ptr) {
			for (int j = 0; j < i; j++)
				free(ptr[j]);
			free(ptr);
		}
	}

	return ptr;
}

void deallocate_matrix(unsigned int **ptr, int n)
{
	for (int i = 0; i < n; i++)
		free(ptr[i]);
	free(ptr);
}

void deallocate_double_matrix(double **ptr, int n)
{
	for (int i = 0; i < n; i++)
		free(ptr[i]);
	free(ptr);
}
