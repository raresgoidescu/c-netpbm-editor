#include <stdio.h>
#include <stdlib.h>

void perr(char *err_message)
{
	fprintf(stdout, "%s", err_message);
	exit(-1);
}
