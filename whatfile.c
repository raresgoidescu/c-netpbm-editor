#include <stdio.h>

int readMagicWord(const char *path, char *mword, int *colored) {
	FILE *f = fopen(path, "r");
    if (!f) {
        printf("Failed to load %s\n", path);
        return 0;
    }
    fscanf(f, "%s\n", mword);
    fclose(f);

    if (mword[1] == '3' || mword[1] == '6')
        *colored = 1;
    return 1;
}