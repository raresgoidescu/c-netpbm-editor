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

int sum_from_0_to_a(int *v, int a) {
    int sum = 0;
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

double **sq_int_double_matrix_multiplication(double **m1, double **m2, int n)
{
    //int **res = allocate_int_matrix(n, n);
    double **tmp = allocate_double_matrix(n, n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            tmp[i][j] = 0.;
            for (int k = 0; k < n; ++k) {
                tmp[i][j] += (double)((double)m1[i][k] * (double)m2[k][j]);
            }
        }
    }

    //deallocate_double_matrix(tmp, n);
    //return res;
    return tmp;
}

void apply_kernel(unsigned int ***src, double **ker, int n)
{
    double **red = allocate_double_matrix(n, n);
    double **green = allocate_double_matrix(n, n);
    double **blue = allocate_double_matrix(n, n);

    int alpha = (*src)[1][1] >> 24;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int b = ((*src)[i][j] >> 16) & alpha;
            int g = ((*src)[i][j] >> 8) & alpha;
            int r = ((*src)[i][j]) & alpha;
            blue[i][j] = b;
            green[i][j] = g;
            red[i][j] = r;
        }
    }

    double **red_ = sq_int_double_matrix_multiplication(red, ker, n);
    for (int i = 0; i < n; i++)
        free(red[i]);
    free(red);

    double **green_ = sq_int_double_matrix_multiplication(green, ker, n);
    for (int i = 0; i < n; i++)
        free(green[i]);
    free(green);

    double **blue_ = sq_int_double_matrix_multiplication(blue, ker, n);
    for (int i = 0; i < n; i++)
        free(blue[i]);
    free(blue);

    double sum_r = 0, sum_g = 0, sum_b = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum_r += red_[i][j];
            sum_g += green_[i][j];
            sum_b += blue_[i][j];
        }
    }

    int r, g, b;
    r = round(sum_r);
    g = round(sum_g);
    b = round(sum_b);
    r = clamp(r);
    g = clamp(g);
    b = clamp(b);
    (*src)[1][1] = alpha << 24 | b << 16 | g << 8 | r;

    for (int i = 0; i < n; i++)
        free(red_[i]);
    free(red_);
    for (int i = 0; i < n; i++)
        free(green_[i]);
    free(green_);
    for (int i = 0; i < n; i++)
        free(blue_[i]);
    free(blue_);
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
}

void crop(img_data *data, int from_x, int from_y, int to_x, int to_y)
{
    int width, height;

    width = to_x - from_x;
    height = to_y - from_y;
    // printf("%d\t|\t%d\n", height, width);
    unsigned int **cropped_map = allocate_matrix(height, width);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cropped_map[i][j] = data->pixel_map[from_y + i][from_x + j];
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

    // printf("%d\t|\t%d\n", height, width);
    deallocate_matrix(cropped_map, height);

    puts("Image cropped");
}

void apply(img_data *data, char *param, int from_x, int from_y, int to_x, int to_y)
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

    unsigned int **newpixel_map = allocate_matrix(data->height, data->width);

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
        puts("APPLY parameter invalid");
        deallocate_double_matrix(kernel, 3);
        return;
    }

    if (from_x == 0)
        from_x = 1;
    if (to_x == data->width)
        to_x--;
    if (from_y == 0)
        from_y = 1;
    if (to_y == data->height)
        to_y--;

    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {

            unsigned int **mblock = allocate_matrix(3, 3);
            for (int k = -1; k < 2; k++) {
                for (int l = -1; l < 2; l++) {
                    mblock[k + 1][l + 1] = data->pixel_map[i + k][j + l];
                }
            }

            //printf("OLD--%u\n", mblock[1][1]);
            apply_kernel(&mblock, kernel, 3);
            //printf("NEW--%u\n", mblock[1][1]);

            unsigned int newpixel = mblock[1][1];

            newpixel_map[i][j] = newpixel;

            deallocate_matrix(mblock, 3);
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
