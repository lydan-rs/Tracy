#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linalg/vec3.h"
#include <math.h>
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



Vec3 skyboxColor(Ray r) {
	float a = 0.5 * (r.direction.y + 1.0);
	return addV3(
			scaleV3((Vec3){1.0,1.0,1.0}, 1.0-a),
			scaleV3((Vec3){0.5,0.7,1.0}, a)
			);
}

typedef enum {
	SPHERE,
	// BOX,
	// PLANE,
	// GRID,
	// CONE,
	// CYLINDER,
	// MESH
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
				if (t < 0.0) { t = (h+discSqrt)/a; } 

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

typedef struct {
	Vec3 position;
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
	camera.position = (Vec3){0,0,0};
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
				Vec3 origin = {
					.x = ((camera._pixDeltaX-camera._sensorWidth)/2) + (camera._pixDeltaX*x),
					.y = ((camera._sensorHeight-camera._pixDeltaY)/2) - (camera._pixDeltaY*y),
					.z = 0.0
				};

				Vec3 direction = normaliseV3(
						(Vec3){
						.x = origin.x,
						.y = origin.y,
						.z = camera.focalLength
						});


				origin = addV3(origin, camera.position);
				Ray ray = {origin, direction};
				return ray;
}

Ray pixelRayWithOffset(Camera camera, unsigned int x, unsigned int y){
				// By multiplying the delta with frand(), we get a random offset within the pixel bounds.
				Vec3 origin = {
					.x = (((camera._pixDeltaX*frand01())-camera._sensorWidth)/2) + (camera._pixDeltaX*x),
					.y = ((camera._sensorHeight-(camera._pixDeltaY*frand01()))/2) - (camera._pixDeltaY*y),
					.z = 0.0
				};

				Vec3 direction = normaliseV3(
						(Vec3){
						.x = origin.x,
						.y = origin.y,
						.z = camera.focalLength
						});


				origin = addV3(origin, camera.position);
				Ray ray = {origin, direction};
				return ray;
}



int main() {
	printf("Hello Bec!\n");

	Image* image = createImage(IMG_HEIGHT, IMG_WIDTH);

	Camera camera = cameraInit(image->width, image->height, 2.0);
	camera.position = (Vec3){0.0, 0.0, 0.0};
	camera.focalLength = 1.0;

	Vec3  sphereCenter = {.x=0, .y=0, .z=2.0};

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
			Vec3 outColor = {0.0,0.0,0.0};
			for (int s = 0; s < nSamples; s++) { 

				Ray ray;
				if (nSamples==1) {
					ray = pixelRay(camera, x, y);
				} else {
					ray = pixelRayWithOffset(camera, x, y);
				}


				Vec3 color = skyboxColor(ray);
				Intersection intersection;

				bool didHit = testIntersection(&sphere, ray, &intersection);

				if (didHit) {
					// Handling Backfaces
					if (dotV3(intersection.surfaceNormal, ray.direction)<=0.0){
						color = intersection.surfaceNormal;
						color = scaleV3(
								addV3(color, (Vec3){1.0, 1.0, 1.0}),
								0.5);
					} else {
						color = (Vec3){
							.r = 1.0,
								.g = 0.25098,
								.b = 0.98431
						};
					}
				}
				outColor = addV3(outColor, color);
			}
			outColor = scaleV3(outColor, 1.0/nSamples);
			setPixel(image, x, y, outColor);
		}
		printf("\r%d/%d scanlines complete. %d remaning.", y+1, image->height, (image->height-y-1));
	}

	printf("\n");

	writePPM(image, "output.ppm");

	freeImage(image);
	image = NULL;
}
