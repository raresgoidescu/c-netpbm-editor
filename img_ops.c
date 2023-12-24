#include "img_struct.h"
#include <stdio.h>

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

    // S-AR PUTEA SA FIE MEDIE --- SUM / (256 / bin)
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

    printf("***%d***\n", 256 / bins);

    for (int i = 0; i < 256; i += 256 / bins) {
        printf("%d\t|\t", freq[i]);
        double nastk = ((double)freq[i] / (double)maxfq) * (double)astks;
        for (int j = 0; j < (int)nastk; ++j) {
            printf("*");
        }
        printf("%lf\n", nastk);
    }
}

void equalize(img_data data, int from_x, int from_y, int to_x, int to_y)
{
    /*
        formula: f(a) = 255 / Area * Sum from 0 to a ( H(i) )
        where:
            - f(a) = new value
            - a    = old value
            - Area = :)
            - H(i) = no pixels with the i value 
    */

    long long area = data.height * data.width;

    int H[256], alpha = 255;
    histogram(data, from_x, from_y, to_x, to_y, H);

    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {
            data.pixel_map[i][j] = ((double)alpha / (double)area) * (double)sum_from_0_to_a(H, data.pixel_map[i][j]);
        }
    }
}