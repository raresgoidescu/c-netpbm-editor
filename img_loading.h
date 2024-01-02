#ifndef image_loading
#define image_loading

#include "img_struct.h"

void load_image(const char *path, char *mword, img_data *data,
				int *colored, int *binary);
void save(img_data data, char *mword, char *path, int ascii, int colored);

#endif