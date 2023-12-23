#include <stdio.h>

void readMagicWord(const char *path, char *mword) {
	FILE *f = fopen(path, "r");
    fgets(mword, sizeof(mword), f);
    fclose(f);
	puts(mword);
}