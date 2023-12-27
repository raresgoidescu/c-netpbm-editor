// Copyright Rares-Stefan Goidescu 312CAb 2023-2024
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "img_ops.c"
#include "mem_ops.c"
#include "whatfile.c"
#include "basic_ops.c"
#include "img_struct.h"
#include "parsing.c"
#include "image_info.c"
#include "image_loading.c"
#define BUFFERMAX 500

int main(void)
{
    char buffer[BUFFERMAX], cmd[200], path[200], save_path[200], param[200];

    char magic_word[3];
    int loaded, colored, binary, ascii, ok_load, all, selected;
    int coords[4];

    img_data data;
    data.height = 0;
    data.width = 0;
    data.alpha = 0;

    while (1) {
        fgets(buffer, BUFFERMAX, stdin);
        parse(cmd, buffer, path, save_path, param,
              &ascii, &ok_load, coords, &all, &selected);
        if (!strcmp(buffer, "EXIT")) {
            if (data.height) {
                deallocate_matrix(data.pixel_map, data.height);
                break;
            } else {
			 	puts("No image loaded");
				break;
			}
        } else if (!strcmp(cmd, "LOAD")) {
            if (ok_load) {
                load_image(path, magic_word, &data, &colored, &binary);

                selected = 0;
                all = 1;
                loaded = 1;
                //printf("***\tl:%d b:%d c:%d\n", loaded, binary, colored);
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
            if (all) {
                coords[0] = 0;
                coords[1] = 0;
                coords[2] = data.width;
                coords[3] = data.height;
            } else {
                
            }
        }
    }
    return 0;
}