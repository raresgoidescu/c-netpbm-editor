#include "mem_ops.c"
#include "pgm.h"
#include "err_handling.c"
#include <stdio.h>
#include <string.h>

void readHeaderPGM(FILE *f_ptr, int *width, int *height, char *mword) {
	fscanf(f_ptr, "%d", height);
	fscanf(f_ptr, "%d", width);
	
	int maxval;
	fscanf(f_ptr, "%d", &maxval);

	if (maxval != 255 && ((strcmp(mword, "P2") == 0) || (strcmp(mword, "P5"))))
		perr("Maxval invalid - trebuie sa fie 255");
}

void readFilePGM(const char *path_to_file, pgm_image *data) {
	FILE *f = fopen(path_to_file, "rb");
	if (!f)
		perr("Eroare la deschiderea fisierlui");

	char magic_word[3];
	fgets(magic_word, sizeof(magic_word), f);
	puts(magic_word);

	readHeaderPGM(f, &data->width, &data->height, magic_word);
	fgetc(f);

	data->image = allocate_matrix(data->height, data->width);
	for (int i = 0; i < data->height; ++i) {
		for (int j = 0; j < data->width; ++j) {
			int a = fgetc(f);
			data->image[i][j] = a;
		}
	}
	fclose(f);
}
