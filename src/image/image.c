#include "image.h"
#include <stdio.h>
#include <stdlib.h>

Image *createImage(unsigned int width, unsigned int height) {
  Image *image = malloc(sizeof(Image));

  if (image == NULL) {
    return NULL;
  }

  image->width = width;
  image->height = height;
  image->data = calloc(width * height, sizeof(Vec3));
	// printf("%d pixels allocated\n", width*height);

  if (image->data == NULL) {
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


IMG_ERROR_CODES writePPM(Image* image, const char* path) {
	
	FILE* file = fopen(path, "w");
	if (file == NULL) {
		printf("Cannot open %s\n", path);
		return FILE_IO;
	}

	printf("Writing Image...\n");

	fprintf(file, "P3\n%d %d\n255\n", image->width, image->height);

	printf("Header Writen\n");

	printf("\r%d/%d scanlines complete. %d remaning.", 0, image->height, image->height);
	for (int r = 0; r < image->height; r++) {
		for (int c = 0; c < image->width; c++) {
			int index = (r*image->width)+c;
			// printf("Array Index %d\n", index);
			Vec3 pixel = image->data[index];

			int red   = (int)(255.66 * pixel.r);
			int green = (int)(255.66 * pixel.g);
			int blue  = (int)(255.66 * pixel.b);

			fprintf(file, "%d %d %d\n", red, green, blue);
		}
		printf("\r%d/%d scanlines complete. %d remaning.", r+1, image->height, (image->height-r-1));
	}
	printf("\n");

	fclose(file);

	printf("Image finished! YAAAY!!!\n");

	return SUCCESS;
}
