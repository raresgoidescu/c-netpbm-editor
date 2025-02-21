// Copyright Rares-Stefan Goidescu 312CAb 2023-2024

#include <stdio.h>
#include <string.h>

/* Functie care returneaza 1 in caz ca imaginea e color */
int is_colored(const char *path, char *mword)
{
	FILE *f = fopen(path, "r");

	fscanf(f, "%s\n", mword);
	fclose(f);

	if (mword[1] == '3' || mword[1] == '6')
		return 1;

	return 0;
}

/* Functie care returneaza 1 in caz ca imaginea este in format raw*/
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
