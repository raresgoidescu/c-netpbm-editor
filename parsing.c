#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************PRIVATE FUNCTIONS******************************/

int file_exists(char *path)
{
	if (path[strlen(path) - 1] == '/') {
		printf("Failed to load %s\n", path);
		return 0;
	}

	FILE * f = fopen(path, "r");

	if (!f) {
		printf("Failed to load %s\n", path);
		return 0;
	}

	char c = '\0';
	int n = '0';
	if (fscanf(f, "%c%d\n", &c, &n)) {
		if (c != 'P') {
			printf("Failed to load %s\n", path);
			return 0;
		}
		if (n < 1 || n > 6) {
			printf("Failed to load %s\n", path);
			return 0;
		}
	}

	fclose(f);
	return 1;
}

int want_ascii(char *param)
{
	if (!param)
		return 0;

	if (!strcmp(param, "ascii"))
		return 1;

	puts("what the duck man");
	return 0;
}

int cmd_not_found(char *cmd)
{
	if (strcmp(cmd, "LOAD") && strcmp(cmd, "SELECT") &&
		strcmp(cmd, "HISTOGRAM") && strcmp(cmd, "EQUALIZE") &&
		strcmp(cmd, "ROTATE") && strcmp(cmd, "CROP") &&
		strcmp(cmd, "APPLY") && strcmp(cmd, "SAVE") && strcmp(cmd, "EXIT")) {
		printf("Invalid command\n");
		return 0;
	}

	return 1;
}

void planb(int *coords, int *bckup)
{
	for (int i = 0; i < 4; i++)
		coords[i] = bckup[i];
}

void verify_selection_syntax(char *p, int coords[], int bckup[], int *selected,
							 int *all, int *select_err, int *field)
{
	char delims[] = "\n ";
	int k = -1;
	while (p) {
		if (!isalpha(p[0])) {
			coords[++k] = atoi(p);
		} else {
			planb(coords, bckup);
			// puts("Invalid command (letter found)");
			puts("Invalid command");
			*all = (*all == 1) ? 1 : 0;
			*selected = (*selected == 1) ? 1 : 0;
			*select_err = 1;
			return;
		}
		(*field)++;
		p = strtok(NULL, delims);
	}
	*select_err = 0;
	if (*field != 5 && !(*select_err)) {
		planb(coords, bckup);
		*all = (*all == 1) ? 1 : 0;
			*selected = (*selected == 1) ? 1 : 0;
		// puts("Invalid command (not enough coords)");
		puts("Invalid command");
		*select_err = 1;
		return;
	}
}

void verify_histogram_syntax(char *p, int *astks, int *bins, int *field)
{
	char delims[] = "\n ";
	if (!p) {
		*astks = -1;
		return;
	}
	*astks = atoi(p);
	(*field)++;
	p = strtok(NULL, delims);
	if (!p) {
		*astks = -1;
		return;
	}
	*bins = atoi(p);
	(*field)++;
	p = strtok(NULL, delims);
	if (p) {
		*astks = -1, *bins = 0;
		return;
	}
}

/*****************************EXPORTED FUNCTIONS******************************/

void parse(char *cmd, char *buffer, char *path, char *save_path, char *param,
		   int *ascii, int *ok_load, int *coords, int *bckup, int *all,
		   int *selected, int *astks, int *bins, int *angle, int *select_err)
{
	char delims[] = "\n ";
	char *p = strtok(buffer, delims);
	int field = 0;

	while (p) {
		if (field == 0) {
			strcpy(cmd, p);
			int cmd_found = cmd_not_found(cmd);
			if (cmd_found == 0)
				return;
			field++;
			p = strtok(NULL, delims);
			continue;
		}
		if (field == 1) {
			if (!strcmp(cmd, "EXIT")) {
				return;
			} else if (!strcmp(cmd, "LOAD")) {
				strcpy(path, p);
				int can_load = file_exists(path);
				*ok_load = (can_load) ? 1 : 0;
				return;
			} else if (!strcmp(cmd, "SELECT")) {
				*selected = 1;
				if (!strcmp(p, "ALL")) {
					*all = 1, field = 5;
					break;
				}
				for (int i = 0; i < 4; i++)
					bckup[i] = coords[i];
				verify_selection_syntax(p, coords, bckup, selected, all,
										select_err, &field);
				*selected = 1, *all = 0;
				break;
			} else if (!strcmp(cmd, "HISTOGRAM")) {
				verify_histogram_syntax(p, astks, bins, &field);
				break;
			} else if (!strcmp(cmd, "EQUALIZE")) {
				return;
			} else if (!strcmp(cmd, "ROTATE")) {
				*angle = atoi(p);
				return;
			} else if (!strcmp(cmd, "CROP")) {
				return;
			} else if (!strcmp(cmd, "APPLY")) {
				if (!p) {
					param[0] = '0';
					return;
				}
				strcpy(param, p);
				field++;
				p = strtok(NULL, delims);
				continue;
			} else if (!strcmp(cmd, "SAVE")) {
				strcpy(save_path, p);
				field++;
				p = strtok(NULL, delims);
				*ascii = want_ascii(p);
				return;
			}
		}
		field++;
		p = strtok(NULL, delims);
		continue;
	}
	if (!strcmp(cmd, "HISTOGRAM") && field == 1)
		*astks = -1;
}
