#include <stdlib.h>
#include <stdio.h>

void handle_null_pointer(void *p) {
	if (!p) {
		printf("Eroare la alocare\n");
		exit(-1);
	}
}

/* O pot face sa fie mai generala (sa mearga si pentru u, si pt d si pt lf) */
unsigned int **allocate_matrix(int n, int m) {
	unsigned int **ptr = NULL;
	ptr = malloc(n * sizeof(unsigned int *));
	handle_null_pointer(ptr);

	for (int i = 0; i < n; ++i) {
		ptr[i] = malloc(m * sizeof(unsigned int));
		handle_null_pointer(ptr[i]);
		if (!ptr) {
			for (int j = 0; j < i; j++) {
				free(ptr[j]);
			}
			free(ptr);
		}
	}

	return ptr;
}

void deallocate_matrix(unsigned int **ptr, int n) {
	for (int i = 0; i < n; i++)
		free(ptr[i]);
	free(ptr);
}
