#include "img_struct.h"
#include "err_handling.c"
#include "mem_ops.c"
#include <stdio.h>
#include <string.h>

/******************************PRIVATE FUNCTIONS******************************/

void histogram(img_data data, int from_x, int from_y, int to_x, int to_y, int *freq)
{
    for (int i = 0; i < 256; i++)
        freq[i] = 0;

    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {
            freq[data.pixel_map[i][j]]++;
        }
    }
}

int sum_from_0_to_a(int *v, int a) {
    int sum = 0;
    for (int i = 0; i < a; i++)
        sum += v[i];
    return sum;
}

/*****************************EXPORTED FUNCTIONS******************************/

/* Nu merge pentru pbm */
void print_histogram(img_data data, int from_x, int from_y, int to_x, int to_y, int astks, int bins, int colored)
{
    /*
        formula = (fq / max(fq)) * astks
    */

    if (colored) {
        puts("Black and white image needed");
        return;
    }

    int freq[256];

    histogram(data, from_x, from_y, to_x, to_y, freq);

    for (int i = 0; i < 256; i += 256 / bins) {
        int sum = 0;
        for (int j = i; j < i + (256 / bins); ++j)
            sum += freq[j];
        for (int j = i; j < i + (256 / bins); ++j)
            freq[j] = sum;
    }

    int maxfq = 0;
    for (int i = 0; i < 256; i += 256 / bins)
        if (freq[i] > maxfq)
            maxfq = freq[i];

    for (int i = 0; i < 256; i += 256 / bins) {
        double nastk = ((double)freq[i] / (double)maxfq) * (double)astks;
        printf("%d\t|\t", (int)nastk);
        for (int j = 0; j < (int)nastk; ++j) {
            printf("*");
        }
        printf("\n");
    }
}

void equalize(img_data data, int from_x, int from_y, int to_x, int to_y, int colored)
{
    /*
        formula: f(a) = 255 / Area * Sum from 0 to a ( H(i) )
        where:
            - f(a) = new value
            - a    = old value
            - Area = :)
            - H(i) = no pixels with the i value 
    */

    if (colored) {
        puts("Black and white image needed");
        return;
    }

    long long area = data.height * data.width;

    int H[256], alpha = 255;
    histogram(data, from_x, from_y, to_x, to_y, H);

    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {
            data.pixel_map[i][j] = ((double)alpha / (double)area) * (double)sum_from_0_to_a(H, data.pixel_map[i][j]);
        }
    }
}

void save(img_data data, char *mword, char *path, int ascii, int colored)
{
    FILE *f;
    if (!ascii) {
        f = fopen(path, "wb");
    } else {
        f = fopen(path, "w");
    }

    if (!f)
        perr("Cant open file");

    // Binary | ASCII | extension
    // -------+-------+----------
    //     P4 |    P1 | 	 .pbm
    //     P5 |    P2 | 	 .pgm
    //     P6 |    P3 | 	 .ppm

    if (colored) {

        if (ascii && !strcmp(mword, "P6"))
            mword[1] = '3';
        if (!ascii && !strcmp(mword, "P3"))
            mword[1] = '6';

        fprintf(f, "%s ", mword);
        fprintf(f, "%d %d ", data.width, data.height);
        fprintf(f, "%d ", data.alpha);

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
                if (!ascii) {
                    fputc(pixel, f);
                } else {
                    fprintf(f, "%d ", pixel);
                }
            }
        }
    }

    fclose(f);
}

void crop(img_data *data, int from_x, int from_y, int to_x, int to_y)
{
    int width, height;
    width = to_x - from_x;
    height = to_y - from_y;
    unsigned int **cropped_map = allocate_matrix(height, width);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cropped_map[i][j] = data->pixel_map[from_x + i][from_y + j];
        }
    }

    deallocate_matrix(data->pixel_map, data->height);

    data->pixel_map = allocate_matrix(height, width);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; ++j)
            data->pixel_map[i][j] = cropped_map[i][j];

    data->height = height;
    data->width = width;

    deallocate_matrix(cropped_map, height);

    puts("Image cropped");
}
