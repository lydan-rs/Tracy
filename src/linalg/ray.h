#ifndef RAY_H
#define RAY_H

#include "../linalg/vector.h"

typedef struct {
	vec3 origin;
	vec3 direction;
} Ray;

Ray   normaliseRay  (Ray r);
vec3  pointAlongRay (Ray r, float t);

typedef struct {
	vec3  position;
	vec3  normal;
	vec2  uv;
	float distance;
} HitData;

bool intersectRaySphere(Ray ray, vec3 center, float radius, HitData* hitData);

#endif
