#include <stdlib.h>
#include <stdio.h>

void handle_null_pointer(void *p) {
	if (!p) {
		printf("Eroare la alocare\n");
		exit(-1);
	}
}

int **allocate_matrix(int n, int m) {
	int **ptr = NULL;
	ptr = malloc(n * sizeof(int *));
	handle_null_pointer(ptr);

	for (int i = 0; i < n; ++i) {
		ptr[i] = malloc(m * sizeof(int));
		handle_null_pointer(ptr[i]);
		for (int j = 0; j < i; j++) {
			free(ptr[j]);
		}
		free(ptr);
	}

	return ptr;
}

void deallocate_matrix(int **ptr, int n) {
	for (int i = 0; i < n; i++)
		free(ptr[i]);
	free(ptr);
}
