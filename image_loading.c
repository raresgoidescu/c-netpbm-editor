#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "img_struct.h"
#include "mem_ops.c"
#include "err_handling.c"

// Binary | ASCII | extension
//     P4 |    P1 | 	 .pbm
//     P5 |    P2 | 	 .pgm
//     P6 |    P3 | 	 .ppm

void nonppm_load(FILE *f_ptr, const int height, const int width, const int binary, img_data *data)
{
    if (binary)
        for (int i = 0; i < height; ++i) {
            for (int  j = 0; j < width; ++j) {
                int pixel = fgetc(f_ptr);
                data->pixel_map[i][j] = pixel;
            }
        }

    if (!binary)
        for (int i = 0; i < height; ++i) {
            for (int  j = 0; j < width; ++j) {
                fscanf(f_ptr, "%d", &data->pixel_map[i][j]);
            }
        }
}

void ppm_load(FILE *f_ptr, const int height, const int width, const int binary, img_data *data)
{
    if (binary)
        for (int i = 0; i < height; ++i) {
            for (int  j = 0; j < width * 3; ++j) {
                int r = fgetc(f_ptr);
                int g = fgetc(f_ptr);
                int b = fgetc(f_ptr);
                data->pixel_map[i][j] = data->alpha << 24 | b << 16 | g << 8 | r;
            }
        }

    if (!binary)
        for (int i = 0; i < height; ++i) {
            for (int  j = 0; j < width * 3; ++j) {
                int r, g, b;
                fscanf(f_ptr, "%d%d%d", &r, &g, &b);
                data->pixel_map[i][j] = data->alpha << 24 | b << 16 | g << 8 | r;
            }
        }
}

void load_image(const char *path, const char *mword,
                img_data *data, int *height, int *width)
{
    FILE *f;
    int binary = 0;
    if (!(strcmp(mword, "P4")) ||
        !(strcmp(mword, "P5")) ||
        !(strcmp(mword, "P6"))) {

        // Binary File (Raw)
        binary = 1;
        f = fopen(path, "rb");
        if (!f)
            perr("Eroare la deschiderea fisierului\n");

    } else if (!(strcmp(mword, "P1")) ||
               !(strcmp(mword, "P2")) ||
               !(strcmp(mword, "P3"))) {

        // ASCII File (Plain text)
        f = fopen(path, "r");
        if (!f)
            perr("Eroare la deschiderea fisierului\n");

    } else {
        perr("Fisier invalid (Nu contine magic word-ul)\n");
    }

    // Citim headerul
    char spare_char;
    if (fscanf(f, "P%c\n", &spare_char) != 1)
        perr("Nu contine magic word-ul\n");

    // Trecem peste comentarii
    // Trebuie pus dupa fiecare linie din header
    spare_char = fgetc(f);
    while (spare_char == '#') {
        spare_char = fgetc(f);
        while (spare_char != '\n') {
            spare_char = fgetc(f);
        }
        spare_char =fgetc(f);
    }

    if (!(isdigit(spare_char)))
        perr("Invalid content of file\n");

    ungetc(spare_char, f);                // Punem cifra inapoi

    int maxval;
    fscanf(f, "%d%d%d\n", width, height, &maxval);
    data->alpha = maxval;

    if (!(strcmp(mword, "P1")) || !(strcmp(mword, "P4")))
        if (maxval != 1)
            perr("Nu este alb negru / pbm\n");
    if (!(strcmp(mword, "P2")) || !(strcmp(mword, "P5")) ||
        !(strcmp(mword, "P3")) || !(strcmp(mword, "P6"))) {
        if (maxval != 255)
            perr("Max val al unui pixel nu este 255 (Lucram cu 255)\n");
    }

    // Binary | ASCII | extension
    //     P4 |    P1 | 	 .pbm
    //     P5 |    P2 | 	 .pgm
    //     P6 |    P3 | 	 .ppm

    if (!(strcmp(mword, "P1")) || !(strcmp(mword, "P4"))) {
        nonppm_load(f, *height, *width, binary, data);
    } else if (!(strcmp(mword, "P2")) || !(strcmp(mword, "P5"))) {
        nonppm_load(f, *height, *width, binary, data);
    } else if (!(strcmp(mword, "P3")) || !(strcmp(mword, "P6"))) {
        ppm_load(f, *height, *width, binary, data);
    }
    fclose(f);
}
