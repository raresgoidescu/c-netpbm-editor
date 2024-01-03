// Copyright Rares-Stefan Goidescu 312CAb 2023-2024

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "img_ops.h"
#include "mem_ops.h"
#include "parsing.h"
#include "img_info.h"
#include "basic_ops.h"
#include "img_struct.h"
#include "img_loading.h"
#define BUFFERMAX 500

void select_all(int coords[], img_data data)
{
	coords[0] = 0;
	coords[1] = 0;
	coords[2] = data.width;
	coords[3] = data.height;
}

/*
	Functie care verifica corectitudinea setului de coordonate introdus
	In caz ca sunt invalide, se intoarce la selectia anterioara
*/
void validate_coords(int coords[], int backupcoords[], img_data data)
{
	if (coords[0] > coords[2]) // tratam cazurile in care nu sunt in ordine asc
		my_swap(&coords[0], &coords[2]);
	if (coords[1] > coords[3])
		my_swap(&coords[1], &coords[3]);

	int valid_selection = 1; // Benefit of the doubt
	for (int i = 0; i < 4; i++) {
		if (coords[i] < 0)
			valid_selection = 0;
	}
	// Veridicam daca selectia este in cadrul imaginii
	if (coords[2] > data.width || coords[3] > data.height)
		valid_selection = 0;

	if (coords[0] == coords[2] || coords[1] == coords[3])
		valid_selection = 0;
	// Verificam daca coordonatele sunt diferite intre ele intrucat
	// selectia este in intervalul [from_x, to_x) iar in caz ca cele doua
	// ar fi egale, ar fi incorect dpdv logic
	int all_eq = 1;
	for (int i = 0; i < 3; i++) {
		if (coords[i] != coords[i + 1])
			all_eq = 0;
	}
	if (!valid_selection || all_eq) {
		puts("Invalid set of coordinates");
		for (int i = 0; i < 4; ++i)
			coords[i] = backupcoords[i];
		return;
	}
	printf("Selected %d %d %d %d\n", coords[0], coords[1],
		   coords[2], coords[3]);
}

/* Functia main se ocupa de procesarea comenzilor inainte de aplicarea lor */
int main(void)
{
	char buffer[BUFFERMAX], cmd[200], path[200], save_path[200], param[200];
	char magic_word[3];
	int loaded = 0, colored, binary, ascii = 0, ok_load = 0, all, selected;
	int astks = 0, bins = 0, angle = 0;
	int coords[4], backupcoords[4], select_err = 0;
	img_data data;
	data.height = 0, data.width = 0, data.alpha = 0, param[0] = '0';

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
			  &astks, &bins, &angle, &select_err); // parsam bufferul
		if (!strcmp(cmd, "LOAD")) {
			if (ok_load) { // in caz ca fisierul exista, iar sintaxa e ok
				load_image(path, magic_word, &data, &colored, &binary);
				selected = 0, all = 1;
				select_all(coords, data); // Selectia default: integrala
				loaded = 1; // Exista o imagine in memorie
			} else {
				if (data.height)
					deallocate_matrix(data.pixel_map, data.height);
				data.height = 0;
				data.width = 0;
				data.alpha = 0;
				loaded = 0;
				binary = 0;
				colored = 0;
				selected = 0; // Ne intoarcem la stadiul initial
			}
		} else if (!loaded) { // Verificam daca exista o imagine incarcata
			all = 1;
			select_all(coords, data);
			astks = 0, bins = 0;
			puts("No image loaded");
		} else if (!strcmp(cmd, "SELECT")) {
			if (!select_err) { // in caz ca sintaxa comenzii este ok
				if (all) {
					select_all(coords, data);
					puts("Selected ALL");
				} else {
					validate_coords(coords, backupcoords, data);
				}
			}
		} else if (!strcmp(cmd, "HISTOGRAM")) {
			if (astks != -1) { // in caz ca sintaxa comenzii e ok
				print_histogram(data, coords[0], coords[1], coords[2],
								coords[3], astks, bins, colored);
			} else {
				puts("Invalid command");
				astks = 0;
			}
		} else if (!strcmp(cmd, "EQUALIZE")) {
			equalize(&data, colored);
		} else if (!strcmp(cmd, "APPLY")) {
			if (param[0] != '0') // In caz ca avem un parametru
				apply(&data, param,
					  coords[0], coords[1], coords[2], coords[3], colored);
			else
				puts("Invalid command");
		} else if (!strcmp(cmd, "ROTATE")) {
			rotate(&data, &coords[0], &coords[1], &coords[2], &coords[3],
				   angle);
		} else if (!strcmp(cmd, "CROP")) {
			crop(&data, &coords[0], &coords[1], &coords[2], &coords[3]);
		} else if (!strcmp(cmd, "SAVE")) {
			save(data, magic_word, save_path, ascii, colored);
		}
	}
	return 0;
}
