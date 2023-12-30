// Copyright Rares-Stefan Goidescu 312CAb 2023-2024
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "img_ops.c"
#include "mem_ops.c"
#include "basic_ops.c"
#include "img_struct.h"
#include "parsing.c"
#include "image_info.c"
#include "image_loading.c"
#define BUFFERMAX 500

void process_cmd(char *buffer, char *cmd, char *path, char *save_path, char *param, int coords[4], int backupcoords[4])
{
    if (!strcmp(cmd, "LOAD")) {
		if (ok_load) {
			load_image(path, magic_word, &data, &colored, &binary);

			selected = 0;
			all = 1;

			coords[0] = 0;
			coords[1] = 0;
			coords[2] = data.width;
			coords[3] = data.height;
			loaded = 1;
			//printf("***\tl:%d b:%d c:%d\n", loaded, binary, colored);
			//printf("***h:%6d w:%6d\n", data.height, data.width);
		} else {
			if (data.height)
				deallocate_matrix(data.pixel_map, data.height);
			data.height = 0;
			data.width = 0;
			data.alpha = 0;
			loaded = 0;
			binary = 0;
			colored = 0;
			selected = 0;
			//printf("***\tl:%d b:%d c:%d\n", loaded, binary, colored);
		}
	} else if (!strcmp(cmd, "SELECT")) {
		if (loaded && !select_rr) {
			if (all) {
				coords[0] = 0;
				coords[1] = 0;
				coords[2] = data.width;
				coords[3] = data.height;
				puts("Selected ALL");
			} else {
				if (coords[0] > coords[2])
					my_swap(&coords[0], &coords[2]);
				if (coords[1] > coords[3])
					my_swap(&coords[1], &coords[3]);

				int valid_selection = 1; // Benefit of the doubt
				for (int i = 0; i < 4; i++) {
					if (coords[i] < 0)
						valid_selection = 0;
				}
				if (coords[2] > data.width || coords[3] > data.height)
					valid_selection = 0;

				if (coords[0] == coords[2] || coords[1] == coords[3])
					valid_selection = 0;

				int all_eq = 1;
				for (int i = 0; i < 3; i++) {
					if (coords[i] != coords[i + 1])
						all_eq = 0;
				}
				if (!valid_selection || all_eq) {
					puts("Invalid set of coordinates");
					for (int i = 0; i < 4; ++i)
						coords[i] = backupcoords[i];
					continue;
				}
				printf("Selected %d %d %d %d\n", coords[0], coords[1],
						coords[2], coords[3]);
			}
		} else {
			all = 1;
			coords[0] = 0;
			coords[1] = 0;
			coords[2] = data.width;
			coords[3] = data.height;
			puts("No image loaded");
		}
	} else if (!strcmp(cmd, "HISTOGRAM")) {
		if (!loaded) {
			astks = 0;
			bins = 0;
			puts("No image loaded");
		} else {
			if (astks != -1) {
				print_histogram(data, coords[0], coords[1], coords[2],
								coords[3], astks, bins, colored);
			} else {
				puts("Invalid command");
				astks = 0;
			}
		}
	} else if (!strcmp(cmd, "EQUALIZE")) {
		if (loaded)
			equalize(&data, colored);
		else
			puts("No image loaded");
	} else if (!strcmp(cmd, "APPLY")) {
		if (loaded) {
			if (param[0] != '0')
				apply(&data, param,
						coords[0], coords[1], coords[2], coords[3], colored);
			else
				puts("Invalid command");
		} else {
			puts("No image loaded");
		}
	} else if (!strcmp(cmd, "ROTATE")) {
		if (loaded)
			rotate(&data, &coords[0], &coords[1], &coords[2], &coords[3],
					angle);
		else
			puts("No image loaded");
	} else if (!strcmp(cmd, "CROP")) {
		if (loaded)
			crop(&data, &coords[0], &coords[1], &coords[2], &coords[3]);
		else
			puts("No image loaded");
	} else if (!strcmp(cmd, "SAVE")) {
		if (loaded)
			save(data, magic_word, save_path, ascii, colored);
		else
			puts("No image loaded");
	}
}

int main(void)
{
	char buffer[BUFFERMAX], cmd[200], path[200], save_path[200], param[200];

	char magic_word[3];
	int loaded = 0, colored, binary, ascii = 0, ok_load = 0, all, selected;
	int astks = 0, bins = 0, angle = 0;
	int coords[4], backupcoords[4];

	int select_err = 0;

	img_data data;
	data.height = 0;
	data.width = 0;
	data.alpha = 0;

	while (1) {
		fgets(buffer, BUFFERMAX, stdin);
		if (!strcmp(buffer, "EXIT")) {
			if (data.height) {
				deallocate_matrix(data.pixel_map, data.height);
				break;
			}
			puts("No image loaded");
			break;
		}
		parse(cmd, buffer, path, save_path, param,
			  &ascii, &ok_load, coords, backupcoords, &all, &selected,
			  &astks, &bins, &angle, &select_err);
		
	}
	return 0;
}
