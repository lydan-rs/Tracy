#ifndef RAY_H
#define RAY_H

#include "../linalg/vec3.h"

typedef struct {
	Vec3 origin;
	Vec3 direction;
} Ray;

Ray   normaliseRay  (Ray r);
Vec3  pointAlongRay (Ray r, float t);

#endif
