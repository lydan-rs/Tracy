#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils/random.h"
#include "linalg/vector.h"
#include "image/image.h"
#include "linalg/ray.h"
#include "renderer/scene_object.h"
#include "renderer/material.h"


/*
 * NOTES:
 * For this project we are using a left handed system.
 * Y-up, Z extends into the screen.
 */

#define INDEX_2D(col, row, width) (col + (row * width))


const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;





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




// ray, hit record, atten color, reflected ray -> Render -> did reflect


void rayColor(int depth, Photon* photon, SceneObject* objects, int lenObjects, Material* materials, int lenMaterials) {
	if (depth < 1) {
		// return (vec3){0.0, 0.0, 0.0};
		return;
	}

	if (photon == NULL
			|| objects == NULL
			|| materials == NULL
		 ) {
		return;
	}

	HitRecord nearestHit;
	bool anyHit = false;

	for (int i = 0; i < lenObjects; i++) {
		SceneObject* obj = &(objects[i]);
		HitRecord record;
		bool didHit = testIntersection(obj, photon->ray, &record);

		float distEpsilon = 0.001;
		if (
				didHit
				&& record.data.distance > distEpsilon
				&& v3_dot(record.data.normal, photon->ray.direction)<=0.0
				) {
			if (!anyHit) {
				anyHit = true;
				nearestHit = record;
			} else {
				if (record.data.distance < nearestHit.data.distance) {
					nearestHit = record;
				}
			}

		}
	}

	if (anyHit && scatter(photon, &nearestHit, objects, materials)) {
			// vec3 newDir = randomDiffusion(nearestHit.data.normal);
			// vec3 newDir = lambertianDiffusion(nearestHit.data.normal);
			// Ray newRay = {nearestHit.data.position, newDir};
			// Ray newRay = {nearestHit.data.position, nearestHit.data.normal};
			// return v3_scale(rayColor(depth-1, newRay, objects, lenObjects), 0.5);
			rayColor(depth-1, photon, objects, lenObjects, materials, lenMaterials);
			return;
	}
	
	vec3 sky = skyboxColor(photon->ray);
	photon->attenuation = v3_mult(photon->attenuation, sky);
	return;
	// return skyboxColor(ray);
}



int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_HEIGHT, IMG_WIDTH);

	Camera camera = cameraInit(image->width, image->height, 2.0);
	camera.position = (vec3){0.0, 0.0, 0.0};
	camera.focalLength = 1.0;
	
	const int NUM_MATERIALS = 4;
	Material* materials = malloc(sizeof(Material)*NUM_MATERIALS);
	// Material materials[NUM_MATERIALS];
	int greyDiffuse = 0;
	int redMetal = 1;
	int greenMetal = 2;

	materials[greyDiffuse] = (Material){
		.id = greyDiffuse,
		.albedo = (vec3){0.5,0.5,0.5},
		.isMetal = false
	};
	materials[redMetal] = (Material){
		.id = redMetal,
		.albedo = (vec3){0.75,0,0},
		.isMetal = true
	};
	materials[greenMetal] = (Material){
		.id = greenMetal,
		.albedo = (vec3){0.05,0.5,0.05},
		.isMetal = true
	};
	
	const int NUM_OBJECTS = 3;
	SceneObject* objects = malloc(sizeof(SceneObject)*NUM_OBJECTS);
	// SceneObject objects[NUM_OBJECTS];
	objects[0] = (SceneObject){
		.id = 0,
		.position = {0.0,0.0,2.0},
		.scale = 1.0,
		.type = SPHERE,
		.materialID = greyDiffuse
	};
	objects[1] = (SceneObject){
		.id = 1,
		.position = {0.0,-51.0,2.0},
		.scale = 50.0,
		.type = SPHERE,
		.materialID = greenMetal
	};
	objects[2] = (SceneObject){
		.id = 2,
		.position = {3.0,0.5,2.0},
		.scale = 1.5,
		.type = SPHERE,
		.materialID = redMetal
	};

	/*
	 * Multisampling
	 */
	const int nSamples = 100;
	const int maxBounces = 5;
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
				Photon photon = {
					.ray = ray,
					.attenuation = (vec3){1,1,1}
				};

				// vec3 color = rayColor(maxBounces, ray, objects, NUM_OBJECTS);
				rayColor(maxBounces, &photon, objects, NUM_OBJECTS, materials, NUM_MATERIALS);
				outColor = v3_add(outColor, photon.attenuation);
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
