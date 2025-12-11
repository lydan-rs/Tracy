#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linalg/vector.h"
#include "image/image.h"
#include "linalg/ray.h"
#include "renderer/scene_object.h"


/*
 * NOTES:
 * For this project we are using a left handed system.
 * Y-up, Z extends into the screen.
 */

#define INDEX_2D(col, row, width) (col + (row * width))


const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;


float frand01() {
	return (float)rand() / RAND_MAX;
}

// TODO: Determine what happens when min is greater than max.
float frandRange(float min, float max) {
	float r = frand01();
	// if (max < min) {
	// 	float temp = min;
	// 	min = max;
	// 	max = temp;
	// }
	return min + (max-min)*r;
}



vec3 skyboxColor(Ray r) {
	float a = 0.5 * (r.direction.y + 1.0);
	return v3_add(
			v3_scale((vec3){1.0,1.0,1.0}, 1.0-a),
			v3_scale((vec3){0.5,0.7,1.0}, a)
			);
}




typedef struct {
	vec3 position;
	float focalLength;
	float _sensorHeight;
	float _sensorWidth;
	float _pixDeltaX;
	float _pixDeltaY;
	unsigned int _resHoriz;
	unsigned int _resVert;
} Camera;

Camera cameraInit(unsigned int horizontalResolution, unsigned int verticalResolution, float sensorWidth){
	Camera camera;
	camera.position = (vec3){0,0,0};
	camera.focalLength = 1.0;

	camera._resHoriz = horizontalResolution;
	camera._resVert = verticalResolution;
	camera._sensorWidth = sensorWidth;
	camera._sensorHeight = sensorWidth / ((float)horizontalResolution/verticalResolution);
	camera._pixDeltaX = camera._sensorWidth/camera._resHoriz;
	camera._pixDeltaY = camera._sensorHeight/camera._resVert;

	return camera;
}

Ray pixelRay(Camera camera, unsigned int x, unsigned int y){
				vec3 origin = {
					.x = ((camera._pixDeltaX-camera._sensorWidth)/2) + (camera._pixDeltaX*x),
					.y = ((camera._sensorHeight-camera._pixDeltaY)/2) - (camera._pixDeltaY*y),
					.z = 0.0
				};

				vec3 direction = v3_normalise(
						(vec3){
						.x = origin.x,
						.y = origin.y,
						.z = camera.focalLength
						});


				origin = v3_add(origin, camera.position);
				Ray ray = {origin, direction};
				return ray;
}

Ray pixelRayWithOffset(Camera camera, unsigned int x, unsigned int y){
				// By multiplying the delta with frand(), we get a random offset within the pixel bounds.
				vec3 origin = {
					.x = (((camera._pixDeltaX*frand01())-camera._sensorWidth)/2) + (camera._pixDeltaX*x),
					.y = ((camera._sensorHeight-(camera._pixDeltaY*frand01()))/2) - (camera._pixDeltaY*y),
					.z = 0.0
				};

				vec3 direction = v3_normalise(
						(vec3){
						.x = origin.x,
						.y = origin.y,
						.z = camera.focalLength
						});


				origin = v3_add(origin, camera.position);
				Ray ray = {origin, direction};
				return ray;
}



int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_HEIGHT, IMG_WIDTH);

	Camera camera = cameraInit(image->width, image->height, 2.0);
	camera.position = (vec3){0.0, 0.0, 0.0};
	camera.focalLength = 1.0;

	vec3  sphereCenter = {.x=0, .y=0, .z=2.0};

	SceneObject sphere = {
		.position = sphereCenter,
		.type = SPHERE
	};

	/*
	 * Multisampling
	 */
	const int nSamples = 1;
	printf("\r%d/%d scanlines complete. %d remaning.", 0, image->height, image->height);
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			vec3 outColor = {0.0,0.0,0.0};
			for (int s = 0; s < nSamples; s++) { 

				Ray ray;
				if (nSamples==1) {
					ray = pixelRay(camera, x, y);
				} else {
					ray = pixelRayWithOffset(camera, x, y);
				}


				vec3 color = skyboxColor(ray);
				Intersection intersection;

				bool didHit = testIntersection(&sphere, ray, &intersection);

				if (didHit) {
					// Handling Backfaces
					if (v3_dot(intersection.hitNormal, ray.direction)<=0.0){
						color = intersection.hitNormal;
						color = v3_scale(
								v3_add(color, (vec3){1.0, 1.0, 1.0}),
								0.5);
					} else {
						color = (vec3){
							.r = 1.0,
								.g = 0.25098,
								.b = 0.98431
						};
					}
				}
				outColor = v3_add(outColor, color);
			}
			outColor = v3_scale(outColor, 1.0/nSamples);
			setPixel(image, x, y, outColor);
		}
		printf("\r%d/%d scanlines complete. %d remaning.", y+1, image->height, (image->height-y-1));
	}

	printf("\n");

	writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;
}
