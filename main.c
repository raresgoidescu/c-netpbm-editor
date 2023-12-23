// Copyright Rares-Stefan Goidescu 312CAb 2023-2024
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "whatfile.c"
#include "img_struct.h"
#include "cmd_parsing.c"
#include "image_loading.c"
#define BUFFERMAX 200

int main(void)
{
	// Vezi probleme la lungimile bufferelor
	char cmd_buffer[BUFFERMAX], cmd[15], path[30], params[15];
	int angle = 0, selection_coords[4], ALL = 0;
	img_data data;
	while (1) {
		int height, width;
		fgets(cmd_buffer, BUFFERMAX, stdin);
		parse_command(cmd_buffer, cmd, path, params, &angle, selection_coords, &ALL);
		if (!(strcmp(cmd, "LOAD"))) {
			char magic_word[3];
			readMagicWord(path, magic_word);
			// Binary | ASCII | extension
			//     P4 |    P1 | 	 .pbm
			//     P5 |    P2 | 	 .pgm
			//     P6 |    P3 | 	 .ppm
			load_image(path, magic_word, &data, &height, &width);

			for (int j = 0; j < width; ++j)
				printf("%u ", data.pixel_map[0][j]);
			printf("\n");

			deallocate_matrix(data.pixel_map, height);
		}
		if (!(strcmp(cmd, "EXIT"))) {
			break;
		}
	}
	puts(cmd);
	puts(path);
	return 0;
}
