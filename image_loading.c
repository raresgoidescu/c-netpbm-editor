#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "ppm.h"
#include "pgm.h"
#include "mem_ops.c"
#include "err_handling.c"

// Binary | ASCII | extension
//     P4 |    P1 | 	 .pbm
//     P5 |    P2 | 	 .pgm
//     P6 |    P3 | 	 .ppm

void load_image(const char *path, const char *mword,
                void *image, int *height, int *width)
{
    FILE *f;
    if (!(strcmp(mword, "P4")) ||
        !(strcmp(mword, "P5")) ||
        !(strcmp(mword, "P6"))) {

        // Binary File (Raw)
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
    fscanf(f, "%d%d%d\n", width, height, &maxval);

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

    
    fclose(f);
}

void pbm_load();

void pgm_load();

void ppm_load();