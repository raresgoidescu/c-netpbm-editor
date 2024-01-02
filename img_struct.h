#ifndef img_struct
#define img_struct

typedef struct img_data {
	int width;
	int height;
	int alpha;
	unsigned int **pixel_map;
	// Folosesc o metoda foarte cunoscuta in image processing: packing
} img_data;

#endif /* img_struct */
