#include <stdio.h>
#include <stdlib.h>
#include "linalg/vec3.h"
#include "image/image.h"
#include "renderer/ray.h"


#define INDEX_2D(col, row, width) (col + (row * width))


const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;



Vec3 rayColor(Ray r) {
	float a = 0.5 * (r.direction.y + 1.0);
	return addV3(
			scaleV3((Vec3){1.0,1.0,1.0}, 1.0-a),
			scaleV3((Vec3){0.5,0.7,1.0}, a)
			);
}



int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_HEIGHT, IMG_WIDTH);

	Vec3  cameraCenter = {.x=0, .y=0, .z=0};
	float focalLength  = 1.0;
	float sensorHeight = 2.0;
	float sensorWidth  = sensorHeight * image->aspect;
	float pixelHeight  = sensorHeight/image->height;
	float pixelWidth   = sensorWidth/image->width;

	Vec3  pixel0 = {
		.x = (pixelWidth-sensorWidth)/2,
		.y = (sensorHeight-pixelHeight)/2,
		.z = focalLength
	};

	// Init Sensor
	Ray* rays = malloc(IMG_WIDTH*IMG_HEIGHT*sizeof(Ray));

	/*
	 * This code models a pin hole camera.
	 * Will be fine for now.
	 * But will want to model a camera with the sensor at the camera origin eventually.
	 */
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {

			Vec3 origin = {
				.x = pixel0.x + (pixelWidth * x),
				.y = pixel0.y - (pixelHeight * y),
				.z = -pixel0.z
			};

			Vec3 direction = normaliseV3(
					subtractV3(origin, cameraCenter)
					);

			// rays[x + (y*image->width)] = (Ray){
			rays[INDEX_2D(x, y, image->width)] = (Ray){
				.origin = origin,
				.direction = direction
			};
		}
	}




	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {

			Ray ray = rays[INDEX_2D(x, y, image->width)];
			// image->data[(r*image->width)+c] = pixel;
			// setPixel(image, x, y, pixel);
			setPixel(image, x, y, rayColor(ray));
		}
	}

	writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;


}
