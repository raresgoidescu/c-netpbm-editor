#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "err_handling.c"

typedef struct pgm_image {
	int width;
	int height;
	int maxval;
	int **matrix;
} pgm_image;

int main(void) {
    pgm_image image;
    FILE *f;
    char mword[] = "P5";
    char path[] = "./images/pgm-640.pgm";
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
    fscanf(f, "%d%d%d\n", &image.width, &image.height, &maxval);

    if (!(strcmp(mword, "P1")) || !(strcmp(mword, "P4")))
        if (maxval != 1)
            perr("Nu este alb negru / pbm\n");
    if (!(strcmp(mword, "P2")) || !(strcmp(mword, "P5")) ||
        !(strcmp(mword, "P3")) || !(strcmp(mword, "P6"))) {
        if (maxval != 255)
            perr("Max val al unui pixel nu este 255 (Lucram cu 255)\n");
    }

    printf("%s\n%d\t%d\t%d\n", mword, image.width, image.height, binary);

    // Binary | ASCII | extension
    //     P4 |    P1 | 	 .pbm
    //     P5 |    P2 | 	 .pgm
    //     P6 |    P3 | 	 .ppm

    image.matrix = malloc(image.height * sizeof(int *));
    for (int i = 0; i < image.height; ++i)
        image.matrix[i] = malloc(image.width * sizeof(int));

    if (binary)
        for (int i = 0; i < image.height; ++i) {
            for (int  j = 0; j < image.width; ++j) {
                int a = fgetc(f);
                image.matrix[i][j] = a;
            }
        }

    if (!binary)
        for (int i = 0; i < image.height; ++i) {
            for (int  j = 0; j < image.width; ++j) {
                fscanf(f, "%d", &image.matrix[i][j]);
            }
        }

    for (int i = 0; i < image.height; ++i) {
        for (int  j = 0; j < image.width; ++j) {
            printf("%d ", image.matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < image.height; ++i)
        free(image.matrix[i]);
    free(image.matrix);

    fclose(f);
    return 0;
}