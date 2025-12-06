#include "image.h"
#include <stdio.h>
#include <stdlib.h>

Image *createImage(unsigned int height, unsigned int width) {
  Image *image = malloc(sizeof(Image));

  if (image == NULL) {
		fprintf(stderr, "Failed to allocate enough memory for image struct.\n");
    return NULL;
  }

  image->height = height;
  image->width = width;
	image->aspect = (float)width/height;
  image->data = calloc(width * height, sizeof(Vec3));
	// printf("%d pixels allocated\n", width*height);

  if (image->data == NULL) {
		fprintf(stderr, "Failed to allocate enough memory for image date.\n");
    free(image);
    return NULL;
  }

  return image;
}

void freeImage(Image *image) {
	if (image == NULL) { return; }
	if (image->data != NULL) {
		free(image->data);
		image->data = NULL;
	}

	free(image);

}


void setPixel(Image* image, unsigned int x, unsigned int y, Vec3 pixel) {
	if (image == NULL) {
		fprintf(stderr, "setPixel() cannot operate on NULL image.\n");
		return;
	}

	if (x >= image->width || y >= image->height) { 
		fprintf(stderr, "Coordinates (%d, %d) are out bounds. Bounds = (0, 0)->(%d, %d).\n",
				x, y, image->width-1, image->height-1);
		return;
	}

	image->data[(y*image->width)+x] = pixel;
}


Vec3 getPixel (Image* image, unsigned int x, unsigned int y){
	if (image == NULL) {
		fprintf(stderr, "setPixel() cannot operate on NULL image.\n");
		return (Vec3){.r=0.0, .g=0.0, .b=0.0};
	}

	if (x >= image->width || y >= image->height) { 
		fprintf(stderr, "Coordinates (%d, %d) are out bounds. Bounds = (0, 0)->(%d, %d).\n",
				x, y, image->width-1, image->height-1);
		return (Vec3){.r=0.0, .g=0.0, .b=0.0};
	}

	return image->data[(y*image->width)+x];
}


void writePPM(Image* image, const char* path) {
	
	FILE* file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "Cannot open %s\n", path);
		// *err = FILE_IO;
		return;
	}

	printf("Writing Image...\n");

	fprintf(file, "P3\n%d %d\n255\n", image->width, image->height);

	printf("Header Writen\n");

	printf("\r%d/%d scanlines complete. %d remaning.", 0, image->height, image->height);
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			// int index = (r*image->width)+c;
			// // printf("Array Index %d\n", index);
			// Vec3 pixel = image->data[index];
			Vec3 pixel = getPixel(image, x, y);

			int red   = (int)(255.66 * pixel.r);
			int green = (int)(255.66 * pixel.g);
			int blue  = (int)(255.66 * pixel.b);

			fprintf(file, "%d %d %d\n", red, green, blue);
		}
		printf("\r%d/%d scanlines complete. %d remaning.", y+1, image->height, (image->height-y-1));
	}
	printf("\n");

	fclose(file);

	printf("Image finished! YAAAY!!!\n");

	return;
}
