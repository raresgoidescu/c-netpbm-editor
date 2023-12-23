// Copyright Rares-Stefan Goidescu 312CAb 2023-2024
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "img_ops.c"
#define BUFFERMAX 200

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

int main(void)
{
	// Vezi probleme la lungimile bufferelor
	char cmd_buffer[BUFFERMAX], cmd[15], path[30], params[15];
	int angle = 0, selection_coords[4], ALL = 0;
	struct _pgmdata pgm_image;
	while (1) {
		fgets(cmd_buffer, BUFFERMAX, stdin);
		parse_command(cmd_buffer, cmd, path, params, &angle, selection_coords, &ALL);
		if (!(strcmp(cmd, "LOAD"))) {
			readfile(path, &pgm_image);
			
			for (int i = 0; i < 1; i++) {
				for (int j = 0; j < pgm_image.width; j++) {
					printf("%d ", pgm_image.image[i][j]);
				}
				printf("\n");
			}
			deallocate_matrix(pgm_image.image, pgm_image.height);

		}
		if (!(strcmp(cmd, "EXIT"))) {
			break;
		}
	}
	printf("angle: %d\n", angle);
	puts(cmd);
	puts(path);
	return 0;
}
