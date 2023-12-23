typedef struct pixel {
	int r;
	int g;
	int b;
} pixel;

typedef struct ppm_img {
	int width;
	int height;
	int maxval;
	pixel **image;
} ppm_img;
