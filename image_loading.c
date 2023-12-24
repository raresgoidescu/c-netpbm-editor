#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img_struct.h"
#include "mem_ops.c"
#include "err_handling.c"

// Binary | ASCII | extension
// -------+-------+----------
//     P4 |    P1 | 	 .pbm
//     P5 |    P2 | 	 .pgm
//     P6 |    P3 | 	 .ppm

void nonppm_load(FILE *f_ptr, const int height, const int width, const int binary, img_data *data)
{
    data->pixel_map = allocate_matrix(height, width);

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
    data->pixel_map = allocate_matrix(height, width);

    if (binary)
        for (int i = 0; i < height; ++i) {
            for (int  j = 0; j < width; ++j) {
                int r = fgetc(f_ptr);
                int g = fgetc(f_ptr);
                int b = fgetc(f_ptr);
                data->pixel_map[i][j] = data->alpha << 24 | b << 16 | g << 8 | r;
            }
        }

    if (!binary)
        for (int i = 0; i < height; ++i) {
            for (int  j = 0; j < width; ++j) {
                int r, g, b;
                fscanf(f_ptr, "%d%d%d", &r, &g, &b);
                data->pixel_map[i][j] = data->alpha << 24 | b << 16 | g << 8 | r;
            }
        }
}



void load_image(const char *path, const char *mword, int *binary,
                img_data *data, int *height, int *width)
{
    FILE *f;

    // printf("***%d\t%d***\n", *height, *width);
    if (*height != 0 || *width != 0) {
        deallocate_matrix(data->pixel_map, *height);
    }

    if ((strcmp(mword, "P4") == 0) ||
        (strcmp(mword, "P5") == 0) ||
        (strcmp(mword, "P6") == 0)) {

        // Binary File (Raw)
        *binary = 1;
        f = fopen(path, "rb");
        if (!f) {
            printf("Failed to load %s\n", path);
        }
    } else if ((strcmp(mword, "P1") == 0) ||
               (strcmp(mword, "P2") == 0) ||
               (strcmp(mword, "P3") == 0)) {

        // ASCII File (Plain text)
        f = fopen(path, "r");
        if (!f){
            printf("Failed to load %s\n", path);
        }
    } else {
        printf("Failed to load %s\n", path);
    }

    // Citim headerul
    char spare_char;
    if (fscanf(f, "P%c\n", &spare_char) != 1) {
        printf("Failed to load %s\n", path);
        fclose(f);
    }

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

    // Invalid content
    if (!(isdigit(spare_char))) {
        printf("Failed to load %s\n", path);
        fclose(f);
    }

    ungetc(spare_char, f);                // Punem cifra inapoi

    int maxval;
    fscanf(f, "%d%d%d\n", width, height, &maxval);
    data->alpha = maxval;

    if (!(strcmp(mword, "P1")) || !(strcmp(mword, "P4")))
        if (maxval != 1) {
            /* Nu e alb negru */
            printf("Failed to load %s\n", path);
            fclose(f);
        }
    if (!(strcmp(mword, "P2")) || !(strcmp(mword, "P5")) ||
        !(strcmp(mword, "P3")) || !(strcmp(mword, "P6"))) {
        if (maxval != 255) {
            /* Maxval diferit de 255 */
            printf("Failed to load %s\n", path);
            fclose(f);
        }
    }

    // Binary | ASCII | extension
    // -------+-------+----------
    //     P4 |    P1 | 	 .pbm
    //     P5 |    P2 | 	 .pgm
    //     P6 |    P3 | 	 .ppm

    if (!(strcmp(mword, "P1")) || !(strcmp(mword, "P4"))) {
        nonppm_load(f, *height, *width, *binary, data);
    } else if (!(strcmp(mword, "P2")) || !(strcmp(mword, "P5"))) {
        nonppm_load(f, *height, *width, *binary, data);
    } else if (!(strcmp(mword, "P3")) || !(strcmp(mword, "P6"))) {
        ppm_load(f, *height, *width, *binary, data);
    }

    printf("Loaded %s\n", path);

    fclose(f);
}
