// Copyright Rares-Stefan Goidescu 312CAb 2023-2024
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERMAX 200


int main(void)
{
	// Vezi probleme la lungimile bufferelor
	char cmd_buffer[BUFFERMAX], cmd[15], path[30], param[15];
	int coords[4], angle;
	fgets(cmd_buffer, BUFFERMAX, stdin);
	char delims[] = " ";
	char *p = strtok(cmd_buffer, delims);
	int field = 0;
	while (p) {
		if(field == 0) {
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
			int x1, x2, y1, y2;
			int MODE = 0;
			if (strcmp(p, "ALL") == 0) {
				MODE = 1;
				field++;
				p = strtok(NULL, delims);
				continue;
			} else {
				x1 = atoi(p);
				field++;
				p = strtok(NULL, delims);
				y1 = atoi(p);
				field++;
				p = strtok(NULL, delims);
				x2 = atoi(p);
				field++;
				p = strtok(NULL, delims);
				y2 = atoi(p);
				field++;
				printf("%d %d %d %d\n", x1, y1, x2, y2);
				p = strtok(NULL, delims);
				continue;
			}
		}
		field++;
		p = strtok(NULL, delims);
	}
	puts(cmd);
	puts(path);
	return 0;
}
