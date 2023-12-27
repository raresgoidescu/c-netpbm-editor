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
    if (strcmp(cmd, "LOAD") && strcmp(cmd, "SELECT") &&
        strcmp(cmd, "HISTOGRAM") && strcmp(cmd, "EQUALIZE") &&
        strcmp(cmd, "ROTATE") && strcmp(cmd, "CROP") &&
        strcmp(cmd, "APPLY") && strcmp(cmd, "SAVE") && strcmp(cmd, "EXIT")) {

        printf("Invalid command\n");
        return 0;

    }

    return 1;
}

/*****************************EXPORTED FUNCTIONS******************************/

void parse(char *cmd, char *buffer, char *path, char *save_path, char *param,
           int *ascii, int *ok_load, int *coords, int *bckup, int *all,
           int *selected, int *astks, int *bins, int *angle)
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
                    if (!isalpha(p[0]) && !isalpha(p[2]) &&
                        !isalpha(p[4]) && !isalpha(p[6])) {

                        for (int k = 0; k < 4; k++) {
                            bckup[k] = coords[k];
                            coords[k] = atoi(p);
                            field++;
                            p = strtok(NULL, delims);
                        }

                        *all = 0;
                        break;
                    } else {
                        puts("Invalid set of coordinates lol");
                    }
                }
            } else if (!strcmp(cmd, "HISTOGRAM")) {
                if (p == NULL) {
                    *astks = -1;
                    return;
                }
                *astks = atoi(p);
                field++;
                p = strtok(NULL, delims);
                if (p == NULL) {
                    *astks = -1;
                    return;
                }
                *bins = atoi(p);
                field++;
                p = strtok(NULL, delims);
                if (p != NULL) {
                    *astks = -1;
                    *bins = 0;
                    return;
                }
                break;
            } else if (!strcmp(cmd, "EQUALIZE")) {
                return;
            } else if (!strcmp(cmd, "ROTATE")) {
                *angle = atoi(p);
                return;
            } else if (!strcmp(cmd, "CROP")) {
                return;
            } else if (!strcmp(cmd, "APPLY")) {
                if (p == NULL) {
                    param[0] = '0';
                    return;
                }
                strcpy(param, p);
                field++;
                p = strtok(NULL, delims);
                continue;
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