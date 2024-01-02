#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "mem_ops.h"
#include "img_info.h"
#include "img_struct.h"
#include "err_handling.h"

// Binary | ASCII | extension
// -------+-------+----------
//	   P4 |	   P1 |		 .pbm
//	   P5 |	   P2 |		 .pgm
//	   P6 |	   P3 |		 .ppm

/******************************PRIVATE FUNCTIONS******************************/

void nonppm_load(FILE *f_ptr, int binary, img_data *data)
{
	data->pixel_map = allocate_matrix(data->height, data->width);

	// In caz ca imaginea este in format raw, o citim caracter cu caracter,
	// iar in caz ca este plain text o citim numar cu numar

	if (binary)
		for (int i = 0; i < data->height; ++i) {
			for (int  j = 0; j < data->width; ++j) {
				int pixel = fgetc(f_ptr);
				data->pixel_map[i][j] = pixel;
			}
		}

	if (!binary)
		for (int i = 0; i < data->height; ++i)
			for (int  j = 0; j < data->width; ++j)
				fscanf(f_ptr, "%d", &data->pixel_map[i][j]);
}

void ppm_load(FILE *f_ptr, int binary, img_data *data)
{
	data->pixel_map = allocate_matrix(data->height, data->width);

	// In caz ca imaginea este in format raw, o citim caracter cu caracter,
	// iar in caz ca este plain text o citim numar cu numar

	// In cazul imaginilor color am folosit o tehnica cunoscuta
	// in image processing, packing

	if (binary)
		for (int i = 0; i < data->height; ++i) {
			for (int  j = 0; j < data->width; ++j) {
				int r = fgetc(f_ptr);
				int g = fgetc(f_ptr);
				int b = fgetc(f_ptr);
				int alph = data->alpha;
				data->pixel_map[i][j] = alph << 24 | b << 16 | g << 8 | r;
			}
		}

	if (!binary)
		for (int i = 0; i < data->height; ++i) {
			for (int  j = 0; j < data->width; ++j) {
				int r, g, b, alph = data->alpha;
				fscanf(f_ptr, "%d%d%d", &r, &g, &b);
				data->pixel_map[i][j] = alph << 24 | b << 16 | g << 8 | r;
			}
		}
}

/******************************EXPORTED FUNCTIONS*****************************/

void load_image(const char *path, char *mword, img_data *data,
				int *colored, int *binary)
{
	FILE *f;

	if (data->height != 0 || data->width != 0)
		deallocate_matrix(data->pixel_map, data->height);
	// Aflam daca imaginea este in format raw/plain color/grayscale
	// pentru a sti cum sa deschidem fisierul
	*binary = is_binary(path, mword);
	*colored = is_colored(path, mword);

	if (*binary) {
		f = fopen(path, "rb");
		if (!f)
			printf("Failed to load %s\n", path);
	} else {
		f = fopen(path, "r");
		if (!f)
			printf("Failed to load %s\n", path);
	}

	// Citim headerul
	char spare_char;
	if (fscanf(f, "P%c\n", &spare_char) != 1) {
		printf("Failed to load %s\n", path);
		fclose(f);
		return;
	}

	// Trecem peste comentarii (Pot fi si intre dimensiuni & maxval?)
	spare_char = fgetc(f);
	while (spare_char == '#') {
		spare_char = fgetc(f);
		while (spare_char != '\n')
			spare_char = fgetc(f);
		spare_char = fgetc(f);
	}

	// Invalid content
	if (!(isdigit(spare_char))) {
		printf("Failed to load %s\n", path);
		fclose(f);
		return;
	}

	ungetc(spare_char, f);                // Punem cifra inapoi

	int w, h, a;
	fscanf(f, "%d%d%d\n", &w, &h, &a);
	data->height = h; data->width = w; data->alpha = a;

	if (!(strcmp(mword, "P1")) || !(strcmp(mword, "P4")))
		if (data->alpha != 1) {
			printf("Failed to load 1%s\n", path);
			fclose(f);
			return;
		}
	if (!(strcmp(mword, "P2")) || !(strcmp(mword, "P5")) ||
		!(strcmp(mword, "P3")) || !(strcmp(mword, "P6")))
		if (data->alpha != 255) {
			printf("Failed to load 2%s\n", path);
			fclose(f);
			return;
		}

	if (*colored)
		ppm_load(f, *binary, data);
	else
		nonppm_load(f, *binary, data);

	printf("Loaded %s\n", path);

	fclose(f);
}

void save(img_data data, char *mword, char *path, int ascii, int colored)
{
	FILE *f;
	if (!ascii)
		f = fopen(path, "wb");
	else
		f = fopen(path, "w");

	if (!f)
		perr("Cant open file");

	// Binary | ASCII | extension
	// -------+-------+----------
	//	   P4 |	   P1 |		 .pbm
	//	   P5 |	   P2 |		 .pgm
	//	   P6 |	   P3 |		 .ppm

	// Operatia inversa a incarcarii, salvarea, se bazeaza pe aceeasi idee,
	// doar ca schimbam magic-word-ul in caz ca se doreste salvarea imaginii
	// sub alt format fata de cel initial
	if (colored) {
		if (ascii && !strcmp(mword, "P6"))
			mword[1] = '3';
		if (!ascii && !strcmp(mword, "P3"))
			mword[1] = '6';

		fprintf(f, "%s\n", mword);
		fprintf(f, "%d %d\n", data.width, data.height);
		fprintf(f, "%d\n", data.alpha);

		for (int i = 0; i < data.height; ++i) {
			for (int j = 0; j < data.width; ++j) {
				int alph = data.pixel_map[i][j] >> 24;
				int blue = (data.pixel_map[i][j] >> 16) & alph;
				int green = (data.pixel_map[i][j] >> 8) & alph;
				int red = (data.pixel_map[i][j]) & alph;
				if (!ascii) {
					fputc(red, f);
					fputc(green, f);
					fputc(blue, f);
				} else {
					fprintf(f, "%d %d %d ", red, green, blue);
				}
			}
			if (ascii)
				fprintf(f, "\n");
		}
	} else {
		if (ascii && !strcmp(mword, "P5"))
			mword[1] = '2';
		if (!ascii && !strcmp(mword, "P2"))
			mword[1] = '5';

		fprintf(f, "%s ", mword);
		fprintf(f, "%d %d ", data.width, data.height);
		fprintf(f, "%d ", data.alpha);

		for (int i = 0; i < data.height; ++i) {
			for (int j = 0; j < data.width; ++j) {
				int pixel = data.pixel_map[i][j];
				if (!ascii)
					fputc(pixel, f);
				else
					fprintf(f, "%d ", pixel);
			}
			if (ascii)
				fprintf(f, "\n");
		}
	}

	fclose(f);

	printf("Saved %s\n", path);
}
