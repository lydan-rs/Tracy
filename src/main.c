#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "linalg/vec3.h"
#include "image/image.h"
#include "renderer/ray.h"


/*
 * NOTES:
 * For this project we are using a left handed system.
 * Y-up, Z extends into the screen.
 */

#define INDEX_2D(col, row, width) (col + (row * width))


const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;



Vec3 skyboxColor(Ray r) {
	float a = 0.5 * (r.direction.y + 1.0);
	return addV3(
			scaleV3((Vec3){1.0,1.0,1.0}, 1.0-a),
			scaleV3((Vec3){0.5,0.7,1.0}, a)
			);
}

typedef struct {
	Vec3 surfacePoint;
	Vec3 surfaceNormal;
} Intersection;

Intersecton



int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_HEIGHT, IMG_WIDTH);

	Vec3  cameraCenter = {.x=0, .y=0, .z=2.5};
	Vec3  sphereCenter = {.x=0, .y=0, .z=5.0};
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
				.z = pixel0.z
			};

			origin = addV3(origin, cameraCenter);

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

			// Vec3  u = subtractV3(sphereCenter, ray.origin);
			// float a = dotV3(ray.direction, ray.direction);
			// // float b = dotV3(scaleV3(ray.direction, -2.0), u);
			// float b = -2.0 * dotV3(ray.direction, u);
			// float c = dotV3(u, u) - 1.0; // Radius is hardcoded to one here.
			// float discriminant = b*b - 4*a*c;

			Vec3  u = subtractV3(sphereCenter, ray.origin);
			float a = magSqrdV3(ray.direction);
			float h = dotV3(ray.direction, u); // Aparently an optimisation?
			float c = magSqrdV3(u) - 1.0; // Radius is hardcarded to one here.
			float discriminant = h*h - a*c; // And another optimisation??
			

			Vec3 outColor = skyboxColor(ray);

			if (discriminant >= 0.0) {
				float discSqrt = sqrtf(discriminant);
				// float t = (-b - discSqrt) / (2.0*a);
				float t = (h-discSqrt)/a; // Again, an optimisation.

				if (t > 0.0) {
					outColor = normaliseV3(subtractV3(pointAlongRay(ray, t), sphereCenter)); // Surface normal
					outColor = scaleV3(
							addV3(outColor, (Vec3){1.0, 1.0, 1.0}),
								0.5);
				}
			}

			setPixel(image, x, y, outColor);
		}
	}

	writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;


}
