#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

typedef enum {
	SPHERE,
} ObjectType;

typedef struct {
	Vec3 position;
	// Transforms
	ObjectType type;
	// ShaderType
	// Material
	// Mesh Data
	// BBox
} SceneObject;

typedef struct {
	Vec3 surfacePoint;
	Vec3 surfaceNormal;
	int  primID;
} Intersection;

bool testSphereIntersection(SceneObject* object, Ray ray, Intersection* intersection){
			if (object == NULL) {
				// TODO: Return Error
				return false;
			}

			if (intersection == NULL) {
				//TODO: Return Error
				return false;
			}

			Vec3  u = subtractV3(object->position, ray.origin);

			//Quadratic formula
			float a = magSqrdV3(ray.direction);
			float h = dotV3(ray.direction, u); // Aparently an optimisation?
			float c = magSqrdV3(u) - 1.0; // Radius is hardcarded to one here.
			float discriminant = h*h - a*c; // And another optimisation??

			if (discriminant >= 0.0) {
				float discSqrt = sqrtf(discriminant);
				// TODO: Do I need to handle the + case as well?
				// TODO: Factor in min and max hit distance.
				/*
				 * We probably only want the nearest intersection.
				 * When the discriminant is positive, it's sqrt will also be positive.
				 * Regardless of the precise value of -b, the smallest value will always be the one where the discriminant is subtracted from it.
				 * So no, we do not need to also compute -b + discriminant.
				 *
				 * But wait, if we are inside the sphere, then the lesser t might actually be negative, right?
				 * If this were the case, then we would also want to check the greater t.
				 * Basically, if the lesser t were negative, but the greater t positive, then we would be inside the sphere.
				 * If rendering the backface this would be important.
				 * 
				 */
				float t = (h-discSqrt)/a; // Again, an optimisation.
				if (t < 0.0) { t = (h+discSqrt)/a; } // Again, an optimisation.

				if (t > 0.0) {
					Vec3 hitPoint = pointAlongRay(ray, t);
					intersection->surfacePoint = hitPoint;
					intersection->surfaceNormal = normaliseV3(subtractV3(hitPoint, object->position));
					intersection->primID = 0;
					return true;
				}
			}
	return false;
}

bool testIntersection(SceneObject* object, Ray ray, Intersection* intersection){
			if (object == NULL) {
				// TODO: Return Error
				return false;
			}

			if (intersection == NULL) {
				//TODO: Return Error
				return false;
			}

			switch (object->type) {
				case SPHERE:
					return testSphereIntersection(object, ray, intersection);
				default:
					// TODO: Return Error
					return false;
			}
}


int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_HEIGHT, IMG_WIDTH);

	Vec3  cameraCenter = {.x=0, .y=0, .z=0.0};
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
				.z = 0
			};

			Vec3 direction = normaliseV3(
					(Vec3){
						.x = origin.x,
						.y = origin.y,
						.z = pixel0.z
						});

			// Vec3 direction = normaliseV3(
			// 		subtractV3(origin, cameraCenter)
			// 		);

			origin = addV3(origin, cameraCenter);


			// rays[x + (y*image->width)] = (Ray){
			rays[INDEX_2D(x, y, image->width)] = (Ray){
				.origin = origin,
				.direction = direction
			};
		}
	}


	SceneObject sphere = {
		.position = sphereCenter,
		.type = SPHERE
	};

	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			Ray ray = rays[INDEX_2D(x, y, image->width)];

			Vec3 outColor = skyboxColor(ray);
			Intersection intersection;

			bool didHit = testIntersection(&sphere, ray, &intersection);

			if (didHit) {
				// Handling Backfaces
				if (dotV3(intersection.surfaceNormal, ray.direction)<=0.0){
					outColor = intersection.surfaceNormal;
					outColor = scaleV3(
							addV3(outColor, (Vec3){1.0, 1.0, 1.0}),
							0.5);
				} else {
					outColor = (Vec3){
						.r = 1.0,
						.g = 0.25098,
						.b = 0.98431
					};
				}
			}

			setPixel(image, x, y, outColor);
		}
	}

	writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;


}
