#ifndef img_ops
#define img_ops

#include "img_struct.h"
#include "err_handling.c"
#include "mem_ops.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

long sum_from_0_to_a(int *v, int a) {
    long sum = 0;
    for (int i = 0; i < a; i++)
        sum += v[i];
    return sum;
}

int clamp(int value) {
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
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

void equalize(img_data *data, int colored)
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

    long long area = data->height * data->width;

    int H[256], alpha = data->alpha;
    histogram(*data, 0, 0, data->width, data->height, H);

    unsigned int **tmp = allocate_matrix(data->height, data->width);

    for (int i = 0; i < data->height; ++i) {
        for (int j = 0; j < data->width; ++j) {
            double res = (double)((double)alpha / (double)area) * (long double)sum_from_0_to_a(H, data->pixel_map[i][j]);
            tmp[i][j] = clamp(round(res));
        }
    }

    for (int i = 0; i < data->height; ++i) {
        for (int j = 0; j < data->width; ++j) {
            data->pixel_map[i][j] = tmp[i][j];
        }
    }

    deallocate_matrix(tmp, data->height);

    puts("Equalize done");
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
                if (!ascii) {
                    fputc(pixel, f);
                } else {
                    fprintf(f, "%d ", pixel);
                }
            }
            if (ascii)
                fprintf(f, "\n");
        }
    }

    fclose(f);

    printf("Saved %s\n", path);
}

void crop(img_data *data, int *from_x, int *from_y, int *to_x, int *to_y)
{
    int width, height;

    width = *to_x - *from_x;
    height = *to_y - *from_y;
    // printf("%d\t|\t%d\n", height, width);
    unsigned int **cropped_map = allocate_matrix(height, width);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cropped_map[i][j] = data->pixel_map[*from_y + i][*from_x + j];
        }
    }

    // printf("%d\t|\t%d\n", data->height, data->width);
    deallocate_matrix(data->pixel_map, data->height);

    data->pixel_map = allocate_matrix(height, width);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; ++j)
            data->pixel_map[i][j] = cropped_map[i][j];

    data->height = height;
    data->width = width;

    *from_x = 0;
    *from_y = 0;
    *to_x = data->width;
    *to_y = data->height;

    // printf("%d\t|\t%d\n", height, width);
    deallocate_matrix(cropped_map, height);

    puts("Image cropped");
}

void apply(img_data *data, char *param, int from_x, int from_y, int to_x, int to_y, int colored)
{
    double edge_mat[3][3] = {{-1., -1., -1.},
                             {-1., 8., -1.},
                             {-1., -1., -1.}};
    double sharpen_mat[3][3] = {{0., -1., 0.},
                               {-1., 5., -1.},
                                {0., -1., 0.}};
    double blur_mat[3][3] = {{1/9., 1/9., 1/9.},
                             {1/9., 1/9., 1/9.},
                             {1/9., 1/9., 1/9.}};
    double g_blur_mat[3][3] = {{1/16., 2/16., 1/16.},
                               {2/16., 4/16., 2/16.},
                               {1/16., 2/16., 1/16.}};


    double **kernel = allocate_double_matrix(3, 3);

    if (!strcmp(param, "EDGE")) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                kernel[i][j] = edge_mat[i][j];
    } else if (!strcmp(param, "SHARPEN")) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                kernel[i][j] = sharpen_mat[i][j];
    } else if (!strcmp(param, "BLUR")) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                kernel[i][j] = blur_mat[i][j];
    } else if (!strcmp(param, "GAUSSIAN_BLUR")) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                kernel[i][j] = g_blur_mat[i][j];
    } else {
        if (!strcmp(param, "")) {
            puts("Invalid command");
            return;
        }
        puts("APPLY parameter invalid");
        return;
    }

    if (!colored) {
        puts("Easy, Charlie Chaplin");
        deallocate_double_matrix(kernel, 3);
        return;
    }

    if (from_x == 0)
        from_x++;
    if (to_x == data->width)
        to_x--;
    if (from_y == 0)
        from_y++;
    if (to_y == data->height)
        to_y--;

    unsigned int **newpixel_map = allocate_matrix(data->height, data->width);

    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {
            int red[3][3], green[3][3], blue[3][3];

            for (int k = 0; k < 3; k++)
                for (int l = 0; l < 3; l++) {
                    int alph = data->pixel_map[i + k - 1][j + l - 1] >> 24;
                    blue[k][l] = (data->pixel_map[i + k - 1][j + l - 1] >> 16) & alph;
                    green[k][l] = (data->pixel_map[i + k - 1][j + l - 1] >> 8) & alph;
                    red[k][l] = (data->pixel_map[i + k - 1][j + l - 1]) & alph;
                }

            double new_red = 0, new_green = 0, new_blue = 0;

            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    new_red += red[k][l] * kernel[k][l];
                    new_green += green[k][l] * kernel[k][l];
                    new_blue += blue[k][l] * kernel[k][l];
                }
            }

            int r, g, b;
            r = round(new_red);
            g = round(new_green);
            b = round(new_blue);

            r = clamp(r);
            g = clamp(g);
            b = clamp(b);

            newpixel_map[i][j] = 0;
            newpixel_map[i][j] = data->alpha << 24 | b << 16 | g << 8 | r;
        }
    }

    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {
            data->pixel_map[i][j] = newpixel_map[i][j];
        }
    }

    deallocate_matrix(newpixel_map, data->height);
    deallocate_double_matrix(kernel, 3);

    printf("APPLY %s done\n", param);
}

#endif
