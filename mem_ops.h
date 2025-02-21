// Copyright Rares-Stefan Goidescu 312CAb 2023-2024

#ifndef mem_ops
#define mem_ops

unsigned int **allocate_matrix(int n, int m);
int **allocate_int_matrix(int n, int m);
double **allocate_double_matrix(int n, int m);

void deallocate_matrix(unsigned int **ptr, int n);
void deallocate_double_matrix(double **ptr, int n);

#endif /* mem_ops */
