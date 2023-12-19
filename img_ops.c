#include "mem_ops.c"
#include <stdio.h>

struct _pgmdata {
	int width;
	int height;
	int maxval;
	int **image;
};

void readfile(const char *path_to_file, struct _pgmdata *data) {
	FILE *f = fopen(path_to_file, "rb");
	if (!f) {
		printf("Eroare la deschiderea fisierului\n");
		exit(-1);
	}

	char magic_word[3];
	fgets(magic_word, sizeof(magic_word), f);
	puts(magic_word);

	fscanf(f, "%d", &data->width);
	fscanf(f, "%d", &data->height);
	fscanf(f, "%d", &data->maxval);
	fgetc(f);
	printf("%d | %d | %d\n", data->width, data->height, data->maxval);

	data->image = allocate_matrix(data->height, data->width);
	for (int i = 0; i < data->height; ++i) {
		for (int j = 0; j < data->width; ++j) {
			int a = fgetc(f);
			data->image[i][j] = a;
		}
	}
	fclose(f);
}
