#include <stdio.h>

int main(void)
{
    int alph = 255;
    int red = 100;
    int green = 100;
    int blue = 100;
    unsigned int lol = alph << 24 | blue << 16 | green << 8 | red;
    printf("%u\n", lol);
    alph = red = green = blue = 0;
    alph = lol >> 24;
    blue = (lol >> 16) & alph;
    green = (lol >> 8) & alph;
    red = (lol) & alph;
    printf("%d\t%d\t%d\n", red, green, blue);
    return 0;
}