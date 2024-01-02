#include <stdio.h>
#include <string.h>

int is_colored(const char *path, char *mword)
{
	FILE *f = fopen(path, "r");

	fscanf(f, "%s\n", mword);
	fclose(f);

	if (mword[1] == '3' || mword[1] == '6')
		return 1;

	return 0;
}

int is_binary(const char *path, char *mword)
{
	FILE *f = fopen(path, "r");

	fscanf(f, "%s\n", mword);
	fclose(f);

	if ((strcmp(mword, "P4") == 0) ||
		(strcmp(mword, "P5") == 0) ||
		(strcmp(mword, "P6") == 0))
		return 1;

	return 0;
}
