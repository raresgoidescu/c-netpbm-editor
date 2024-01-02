#include <stdio.h>
#include <stdlib.h>

/* Printeaza un mesaj de eroare cu exit status -1 */
void perr(char *err_message)
{
	fprintf(stdout, "%s", err_message);
	exit(-1);
}
