#include <stdio.h>

void readMagicWord(const char *path, char *mword, int *colored) {
	FILE *f = fopen(path, "r");
    fscanf(f, "%s\n", mword);
    fclose(f);
	puts(mword);
    if (mword[1] == '3' || mword[1] == '6')
        *colored = 1;
}