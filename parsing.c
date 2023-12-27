#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img_ops.c"
#include "image_loading.c"

/******************************PRIVATE FUNCTIONS******************************/

int file_exists(char *path)
{
    if (path[strlen(path) - 1] == '/') {
        printf("Failed to load %s\n", path);
        return 0;
    }

    FILE *f = fopen(path, "r");

    if (!f) {
        printf("Failed to load %s\n", path);
        return 0;
    }

    char c = '\0';
    int n = '0';
    if (fscanf(f, "%c%d\n", &c, &n)) {
        if (c != 'P') {
            printf("Failed to load %s\n", path);
            return 0;
        }
        if (n < 1 || n > 6) {
            printf("Failed to load %s\n", path);
            return 0;
        }
    }

    fclose(f);
    return 1;
}

int want_ascii(char *param)
{
    if (param == NULL)
        return 0;

    if (!strcmp(param, "ascii"))
        return 1;

    puts("what the duck man");
    return 0;
}

int cmd_not_found(char *cmd)
{
    if (strcmp(cmd, "LOAD") && strcmp(cmd, "SELECT") && strcmp(cmd, "HISTOGRAM") &&
				strcmp(cmd, "EQUALIZE") && strcmp(cmd, "ROTATE") && strcmp(cmd, "CROP") &&
				strcmp(cmd, "APPLY") && strcmp(cmd, "SAVE") && strcmp(cmd, "EXIT")) {

        printf("Invalid command\n");
        return 0;

    }

    return 1;
}

/*****************************EXPORTED FUNCTIONS******************************/

void parse(char *cmd, char *buffer, char *path, char *save_path, char *param,
           int *ascii, int *ok_load, int *coords, int *all, int *selected)
{
    int lenght = strlen(buffer) - 1;
	buffer[lenght] = '\0';

    char delims[] = " \n";

    char *p = strtok(buffer, delims);
	int field = 0;

    while (p) {
        if (field == 0) {
			strcpy(cmd, p);

            int cmd_found = cmd_not_found(cmd);
            if (cmd_found == 0)
                return;

			field++;
			p = strtok(NULL, delims);
			continue;
		}

        if (field == 1) {

            if (!strcmp(cmd, "EXIT")) {
                return;
            } else if (!strcmp(cmd, "LOAD")) {
                strcpy(path, p);

                int can_load = file_exists(path);
                *ok_load = 0;

                if (can_load) {
                    *ok_load = 1;
                    return;
                }

                return;
            } else if (!strcmp(cmd, "SELECT")) {
                *selected = 1;
                if (!strcmp(p, "ALL")) {
                    *all = 1;
                    break;
                } else {
                    *all = 0;
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
                    break;
                }
            } else if (!strcmp(cmd, "HISTOGRAM")) {

            } else if (!strcmp(cmd, "EQUALIZE")) {

            } else if (!strcmp(cmd, "ROTATE")) {

            } else if (!strcmp(cmd, "CROP")) {

            } else if (!strcmp(cmd, "APPLY")) {

            } else if (!strcmp(cmd, "SAVE")) {
                strcpy(save_path, p);

                field++;
                p = strtok(NULL, delims);

                *ascii = want_ascii(p);

                return;
            }

        }

        field++;
        p = strtok(NULL, delims);
        continue;
    }
}