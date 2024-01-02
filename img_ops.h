// Copyright Rares-Stefan Goidescu 312CAb 2023-2024

#ifndef img_ops
#define img_ops

#include "img_struct.h"

void print_histogram(img_data data, int from_x, int from_y, int to_x, int to_y,
					 int astks, int bins, int colored);
void equalize(img_data *data, int colored);
void crop(img_data *data, int *from_x, int *from_y, int *to_x, int *to_y);
void apply(img_data *data, char *param, int from_x, int from_y,
		   int to_x, int to_y, int colored);
void rotate(img_data *data, int *from_x, int *from_y, int *to_x, int *to_y,
			int angle);

#endif /* img_ops */
