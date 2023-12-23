#ifndef PPM_H
#define PPM_H

typedef struct img_data {
	int width;
	int height;
	int alpha;
	unsigned int **pixel_map;
} img_data;

#endif /* PPM_H */
