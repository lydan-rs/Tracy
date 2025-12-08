#include "ray.h"


Ray normaliseRay(Ray r) {
	return (Ray){
		.origin = r.origin,
		.direction = v3_normalise(r.direction)
	};
}

vec3  pointAlongRay (Ray r, float t) {
	return v3_add( r.origin, v3_scale(r.direction, t));
}
