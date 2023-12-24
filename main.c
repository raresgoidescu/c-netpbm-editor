// Copyright Rares-Stefan Goidescu 312CAb 2023-2024
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "img_ops.c"
#include "whatfile.c"
#include "basic_ops.c"
#include "img_struct.h"
#include "cmd_parsing.c"
#include "image_loading.c"
#define BUFFERMAX 200

int main(void)
{
	// Vezi probleme la lungimile bufferelor
	char cmd_buffer[BUFFERMAX], cmd[15], path[30], params[15];
	int angle = 0, selection_coords[4], ALL = 0, loaded = 0;
	int selected = 0, astks = 0, bins = 0, colored = 0;
	int height = 0, width = 0;
	img_data data;
	for (int i = 0; i < 4; ++i)
		selection_coords[i] = 0;
	while (1) {
		fgets(cmd_buffer, BUFFERMAX, stdin);
		parse_command(cmd_buffer, cmd, path, params, &angle, selection_coords, &ALL, &loaded, &astks, &bins);
		if (!(strcmp(cmd, "LOAD"))) {
			selected = 0;
			puts(path);
			char magic_word[3];
			readMagicWord(path, magic_word, &colored);
			// Binary | ASCII | extension
			// -------+-------+----------
			//     P4 |    P1 | 	 .pbm
			//     P5 |    P2 | 	 .pgm
			//     P6 |    P3 | 	 .ppm


			load_image(path, magic_word, &data, &height, &width);

			/*
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j)
					printf("%u ", data.pixel_map[i][j]);
				printf("\n");
			}
			*/
		} else if (!(strcmp(cmd, "SELECT"))) {
			if (loaded) {
				selected = 1;
				int ok = 1;
				for (int i = 0; i < 4; ++i)
					if (selection_coords[i] < 0 && ALL == 0)
						ok = 0;
				if (!ok) {
					printf("Invalid set of coordinates\n");
					continue;
				} else {
					if (selection_coords[0] > selection_coords[2])
						my_swap(&selection_coords[0], &selection_coords[2]);
					if (selection_coords[1] > selection_coords[3])
						my_swap(&selection_coords[1], &selection_coords[3]);
					if (ALL) {
						selection_coords[0] = 0;
						selection_coords[1] = 0;
						selection_coords[2] = width;
						selection_coords[3] = height;
					}
				}
				if (!ALL)
					printf("Selected %d %d %d %d\n", selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
				else
				 	printf("Selected ALL\n");
			} else {
				printf("No image loaded\n");
			}
		} else if (!(strcmp(cmd, "HISTOGRAM"))) {
			// Trebuie pus inainte de fiecare comanda
			if (!selected) {
				selection_coords[0] = 0;
				selection_coords[1] = 0;
				selection_coords[2] = width;
				selection_coords[3] = height;
			}

			histogram(data, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3], astks, bins, colored);

		} else if (!(strcmp(cmd, "EXIT"))) {
			if (loaded) {
				deallocate_matrix(data.pixel_map, height);
				break;
			}
			else
			 	printf("No image loaded\n");
		}
	}
	printf("%d\t%d\t%d\t%d\n", selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
	puts(path);
	return 0;
}
