#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Trebuie sa validez si comenzile cu parametri, in caz ca parametrii nu sunt valizi */
/* Trebuie sa maresc lungimile bufferelor */
void parse_command(char buff[], char cmd[], char path[], char params[], char oldpath[], int *angle, int coords[], int *ALL, int *loaded, int *astks, int *bins, int *ascii, int *l_err) {
	int lenght = strlen(buff) - 1;
	// printf("%d\n", lenght);
	buff[lenght] = '\0';
	char delims[] = " ";
	char *p = strtok(buff, delims);
	int field = 0;
	while (p) {
		if (field == 0) {
			strcpy(cmd, p);
			//puts(cmd);
			//printf("***\n%d\n***\n", !strcmp(cmd, "LOAD"));
			if (strcmp(cmd, "LOAD") && strcmp(cmd, "SELECT") && strcmp(cmd, "HISTOGRAM") &&
				strcmp(cmd, "EQUALIZE") && strcmp(cmd, "ROTATE") && strcmp(cmd, "CROP") &&
				strcmp(cmd, "APPLY") && strcmp(cmd, "SAVE") && strcmp(cmd, "EXIT")) {

				printf("Invalid command\n");
				return;
			}
			field++;
			p = strtok(NULL, delims);
			continue;
		} else if (field == 1 && strcmp(cmd, "LOAD") == 0) {
			*l_err = 0;
			if (*loaded)
				strcpy(oldpath, path);
			strcpy(path, p);
			if (path[strlen(path) - 1] == '/') {
				*l_err = 1;
				field++;
				p = strtok(NULL, delims);
				continue;
			}
			*loaded = 1;
			field++;
			p = strtok(NULL, delims);
			continue;
		} else if (field == 1 && strcmp(cmd, "SELECT") == 0) {
			if (strcmp(p, "ALL") == 0) {
				strcpy(params, p);
				*ALL = 1;
				field++;
				p = strtok(NULL, delims);
				continue;
			} else if (!isalpha(p[0]) && !isalpha(p[2]) && !isalpha(p[4]) && !isalpha(p[6])) {
				coords[0] = atoi(p); // x1
				field++;
				p = strtok(NULL, delims);
				coords[1] = atoi(p); // y1
				field++;
				p = strtok(NULL, delims);
				coords[2] = atoi(p); // x2
				field++;
				p = strtok(NULL, delims);
				coords[3] = atoi(p); // y2
				field++;
				// printf("%d %d %d %d\n", coords[0], coords[1], coords[2], coords[3]);
				*ALL = 0;
				p = strtok(NULL, delims);
				continue;
			} else {
				puts("What the duck man");
				field++;
				p = strtok(NULL, delims);
				continue;
			}
		} else if (field == 1 && strcmp(cmd, "HISTOGRAM") == 0) {
			*astks = atoi(p); // x
			field++;
			p = strtok(NULL, delims);
			*bins = atoi(p); // y
			field++;
			p = strtok(NULL, delims);
			continue;
		} else if (field ==  1 && strcmp(cmd, "ROTATE") == 0) {
			*angle = 0;
			*angle = atoi(p);
			field++;
			p = strtok(NULL, delims);
			continue;
		} else if (field == 1 && strcmp(cmd, "APPLY") == 0) {
			strcpy(params, p);
			field++;
			p = strtok(NULL, delims);
			continue;
		} else if (field == 1 && strcmp(cmd, "SAVE") == 0) {
			strcpy(path, p);
			field++;
			p = strtok(NULL, delims);
			if (p != NULL)
				if (strcmp(p, "ascii")) {
					*ascii = 1;
					field++;
					p = strtok(NULL, delims);
					continue;
				}
			continue;
		}
		field++;
		p = strtok(NULL, delims);
	}
}
