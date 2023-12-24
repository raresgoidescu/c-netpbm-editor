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

void check_if_selected(int selected, int *selection_coords, int width, int height) {
	if (!selected) {
		selection_coords[0] = 0;
		selection_coords[1] = 0;
		selection_coords[2] = width;
		selection_coords[3] = height;
	}
}

int main(void)
{
	// Vezi probleme la lungimile bufferelor
	char cmd_buffer[BUFFERMAX], cmd[15], path[30], params[15];
	int angle = 0, selection_coords[4], ALL = 0, loaded = 0;
	int selected = 0, astks = 0, bins = 0, colored = 0, ascii = 0;
	int height = 0, width = 0;
	img_data data;
	for (int i = 0; i < 4; ++i)
		selection_coords[i] = 0;
	while (1) {
		fgets(cmd_buffer, BUFFERMAX, stdin);
		parse_command(cmd_buffer, cmd, path, params, &angle, selection_coords, &ALL, &loaded, &astks, &bins, &ascii);
		if (!(strcmp(cmd, "LOAD"))) {
			selected = 0;
			// puts(path);
			char magic_word[3];
			if (!loaded)
				continue;
			int realfile = readMagicWord(path, magic_word, &colored);
			if (!realfile)
				continue;

			// Binary | ASCII | extension
			// -------+-------+----------
			//     P4 |    P1 | 	 .pbm
			//     P5 |    P2 | 	 .pgm
			//     P6 |    P3 | 	 .ppm

			load_image(path, magic_word, &data, &height, &width);

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
				 	puts("Selected ALL");
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "HISTOGRAM"))) {
			if (loaded) {
				// Trebuie pus inainte de fiecare comanda
				check_if_selected(selected, selection_coords, width, height);

				print_histogram(data, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3], astks, bins, colored);
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "EQUALIZE"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, width, height);

				equalize(data, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3], colored);
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "CROP"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, width, height);

				// save();

				puts("Image cropped");
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "APPLY"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, width, height);

				// save();

				puts("APPLY Done");
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "SAVE"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, width, height);

				// save();

				printf("Saved %s", path);
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "ROTATE"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, width, height);

				// save();

				puts("ROTATE");
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "EXIT"))) {
			if (loaded) {
				deallocate_matrix(data.pixel_map, height);
				break;
			} else {
			 	puts("No image loaded");
			}
		}
	}
	//printf("%d\t%d\t%d\t%d\n", selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
	//puts(path);
	return 0;
}
