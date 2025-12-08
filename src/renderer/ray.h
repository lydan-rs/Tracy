#ifndef RAY_H
#define RAY_H

#include "../linalg/vector.h"

typedef struct {
	vec3 origin;
	vec3 direction;
} Ray;

Ray   normaliseRay  (Ray r);
vec3  pointAlongRay (Ray r, float t);

#endif
