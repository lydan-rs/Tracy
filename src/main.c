#include <stdio.h>
#include "linalg/vec3.h"
#include "image/image.h"

const int IMG_WIDTH = 256;
const int IMG_HEIGHT = 256;

typedef float Vec4[4];

int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_WIDTH, IMG_HEIGHT);


	for (int r = 0; r < image->height; r++) {
		for (int c = 0; c < image->width; c++) {
			Vec3 pixel = {
				.r = (float)c / image->width,
				.g = (float)r / image->height,
				.b = 0.0,
			};

			image->data[(r*image->width)+c] = pixel;
		}
	}

	IMG_ERROR_CODES code = writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;


}
