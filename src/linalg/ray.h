#ifndef RAY_H
#define RAY_H

#include "../linalg/vector.h"

typedef struct {
	vec3 origin;
	vec3 direction;
} Ray;

Ray   normaliseRay  (Ray r);
vec3  pointAlongRay (Ray r, float t);


bool intersectRaySphere(Ray ray, vec3 center, float radius, float* distance, vec3* hitPoint, vec3* hitNormal, vec2* hitUV);

#endif
