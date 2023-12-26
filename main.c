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

/*
	TODO:
		Refactor parsing and main
		Vezi linia 27 in 22
*/


void check_if_selected(int selected, int *selection_coords, img_data data) {
	if (!selected) {
		selection_coords[0] = 0;
		selection_coords[1] = 0;
		selection_coords[2] = data.width;
		selection_coords[3] = data.height;
	}
}

int main(void)
{
	// Vezi probleme la lungimile bufferelor
	char cmd_buffer[BUFFERMAX], cmd[15], path[30], params[15], oldpath[30];
	int angle = 0, selection_coords[4], ALL = 0, loaded = 0, l_err = 0;
	int selected = 0, astks = 0, bins = 0, colored = 0, ascii = 0;
	int height = 0, width = 0, binary = 0, oldcoord[4];
	char magic_word[3], savepath[30];
	img_data data;
	for (int i = 0; i < 4; ++i)
		selection_coords[i] = 0;
	while (1) {
		fgets(cmd_buffer, BUFFERMAX, stdin);
		if (!strcmp(cmd_buffer, "EXIT")) {
			if (loaded) {
				deallocate_matrix(data.pixel_map, data.height);
				break;
			} else {
			 	puts("No image loaded");
				break;
			}
		}
		parse_command(cmd_buffer, cmd, path, params, oldpath,  &angle, selection_coords, &ALL, &loaded, &astks, &bins, &ascii, &l_err, savepath, oldcoord);
		if (!(strcmp(cmd, "LOAD"))) {
			selected = 0;
			colored = 0;
			binary = 0;
			// puts(path);
			if (l_err) {
				printf("Failed to load %s\n", path);
				continue;
			}
			if (!loaded)
				continue;
			int realfile = readMagicWord(path, magic_word, &colored);
			if (!realfile) {
				loaded = 0;
				continue;
			}

			// Binary | ASCII | extension
			// -------+-------+----------
			//     P4 |    P1 | 	 .pbm
			//     P5 |    P2 | 	 .pgm
			//     P6 |    P3 | 	 .ppm
			if (!l_err) {
				//printf("%d\n", l_err);
				load_image(path, magic_word, &binary, &data, &height, &width);
			}
			data.height = height;
			data.width = width;
			selection_coords[0] = 0;
			selection_coords[1] = 0;
			selection_coords[2] = width;
			selection_coords[3] = height;
			oldcoord[0] = 0;
			oldcoord[1] = 0;
			oldcoord[2] = width;
			oldcoord[3] = height;
			// printf("%d | %d \n", data.height, data.width);

		} else if (!(strcmp(cmd, "SELECT"))) {
			if (loaded) {
				selected = 1;
				int ok = 1;
				for (int i = 0; i < 4; ++i)
					if (selection_coords[i] < 0 && ALL == 0)
						ok = 0;
				if (selection_coords[2] > data.width || selection_coords[3] > data.height)
					ok = 0;
				if (!ok) {
					printf("Invalid set of coordinates\n");
					selection_coords[0] = oldcoord[0];
					selection_coords[1] = oldcoord[1];
					selection_coords[2] = oldcoord[2];
					selection_coords[3] = oldcoord[3];
					continue;
				} else {
					if (selection_coords[0] > selection_coords[2])
						my_swap(&selection_coords[0], &selection_coords[2]);
					if (selection_coords[1] > selection_coords[3])
						my_swap(&selection_coords[1], &selection_coords[3]);
					if (ALL) {
						selection_coords[0] = 0;
						selection_coords[1] = 0;
						selection_coords[2] = data.width;
						selection_coords[3] = data.height;
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
				check_if_selected(selected, selection_coords, data);

				print_histogram(data, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3], astks, bins, colored);
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "EQUALIZE"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, data);

				equalize(data, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3], colored);
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "CROP"))) {
			if (loaded) {
				//printf("%d\t%d\t%d\t%d\n", selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
				crop(&data, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
				selection_coords[0] = 0;
				selection_coords[1] = 0;
				selection_coords[2] = data.width;
				selection_coords[3] = data.height;
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "APPLY"))) {
			if (loaded) {
				if (!colored) {
					puts("Easy, Charlie Chaplin");
				} else {
				check_if_selected(selected, selection_coords, data);

				apply(&data, params, selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
				}
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "SAVE"))) {
			if (loaded) {
				check_if_selected(selected, selection_coords, data);

				save(data, magic_word, savepath, ascii, colored);

				printf("Saved %s\n", savepath);
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "ROTATE"))) {
			if (loaded) {
				// check_if_selected(selected, selection_coords, data);

				// save();

				puts("ROTATE not implemented");
			} else {
				puts("No image loaded");
			}
		} else if (!(strcmp(cmd, "EXIT"))) {
			if (loaded) {
				deallocate_matrix(data.pixel_map, data.height);
				break;
			} else {
			 	puts("No image loaded");
			}
		}
	}
	// printf("%d\t%d\t%d\t%d\n", selection_coords[0], selection_coords[1], selection_coords[2], selection_coords[3]);
	// puts(path);
	return 0;
}
