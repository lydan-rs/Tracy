#include "ray.h"


Ray normaliseRay(Ray r) {
	return (Ray){
		.origin = r.origin,
		.direction = normaliseV3(r.direction)
	};
}

Vec3  pointAlongRay (Ray r, float t) {
	return addV3( r.origin, scaleV3(r.direction, t));
}
