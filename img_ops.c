#include "img_struct.h"
#include <stdio.h>

/* Nu merge pentru pbm */
void histogram(img_data data, int from_x, int from_y, int to_x, int to_y, int astks, int bins, int colored)
{
    /*
        formula = (fq / max(fq)) * astks
    */

    if (colored)
        return;

    int freq[256];

    for (int i = 0; i < 256; i++)
        freq[i] = 0;

    // puts("Merge pana aici(1)");
    for (int i = from_y; i < to_y; ++i) {
        for (int j = from_x; j < to_x; ++j) {
            freq[data.pixel_map[i][j]]++;
        }
    }
    // puts("Merge pana aici(2)");

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
