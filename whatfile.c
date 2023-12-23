#include <stdio.h>

void readMagicWord(const char *path, char *mword) {
	FILE *f = fopen(path, "r");
    fscanf(f, "%s\n", mword);
    fclose(f);
	puts(mword);
}