#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_command(char buff[], char cmd[], char path[], char params[], int *angle, int coords[], int *ALL) {
	int lenght = strlen(buff) - 1;
	printf("%d\n", lenght);
	buff[lenght] = '\0';
	char delims[] = " ";
	char *p = strtok(buff, delims);
	int field = 0;
	while (p) {
		if (field == 0) {
			strcpy(cmd, p);
			field++;
			p = strtok(NULL, delims);
			continue;
		}
		if (field == 1 && strcmp(cmd, "LOAD") == 0) {
			strcpy(path, p);
			field++;
			p = strtok(NULL, delims);
			continue;
		}
		if (field == 1 && strcmp(cmd, "SELECT") == 0) {
			if (strcmp(p, "ALL") == 0) {
				strcpy(params, p);
				*ALL = 1;
				field++;
				p = strtok(NULL, delims);
				continue;
			} else {
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
				printf("%d %d %d %d\n", coords[0], coords[1], coords[2], coords[3]);
				p = strtok(NULL, delims);
				continue;
			}
		}
		if (field == 1 && strcmp(cmd, "HISTOGRAM") == 0) {
			coords[0] = atoi(p); // x
			field++;
			p = strtok(NULL, delims);
			coords[1] = atoi(p); // y
			field++;
			p = strtok(NULL, delims);
			continue;
		}
		if (field ==  1 && strcmp(cmd, "ROTATE") == 0) {
			*angle = 0;
			*angle = atoi(p);
			field++;
			p = strtok(NULL, delims);
			continue;
		}
		if (field == 1 && strcmp(cmd, "APPLY") == 0) {
			strcpy(params, p);
			field++;
			p = strtok(NULL, delims);
			continue;
		}
		if (field == 1 && strcmp(cmd, "SAVE") == 0) {
			strcpy(path, p);
			field++;
			p = strtok(NULL, delims);
		}
		field++;
		p = strtok(NULL, delims);
	}
}
