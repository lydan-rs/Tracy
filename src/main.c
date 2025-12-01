#include <stdio.h>
#include "linalg/vec3.h"
#include "image/image.h"

const int IMG_WIDTH = 256;
const int IMG_HEIGHT = 256;

typedef float Vec4[4];

int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_WIDTH, IMG_HEIGHT);


	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			Vec3 pixel = {
				.r = (float)x / image->width,
				.g = (float)y / image->height,
				.b = 0.0,
			};

			// image->data[(r*image->width)+c] = pixel;
			setPixel(image, y, x, pixel);
		}
	}

	writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;


}
