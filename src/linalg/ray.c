#include "ray.h"
#include <math.h>
#include <stdlib.h>


Ray normaliseRay(Ray r) {
	return (Ray){
		.origin = r.origin,
		.direction = v3_normalise(r.direction)
	};
}

vec3  pointAlongRay (Ray r, float t) {
	return v3_add( r.origin, v3_scale(r.direction, t));
}

bool intersectRaySphere(Ray ray, vec3 center, float radius, HitData* hitData){
			vec3  u = v3_sub(center, ray.origin);

			//Quadratic formula
			float a = v3_mag_sqrd(ray.direction);
			float h = v3_dot(ray.direction, u); // Aparently an optimisation?
			float c = v3_mag_sqrd(u) - (radius*radius);
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
				// TODO: Fix t selection. Need the t that is as close to 0 in the positive space (if it exists).
				float t = (h-discSqrt)/a; // Again, an optimisation.
				if (t < 0.0) { t = (h+discSqrt)/a; } 

				if (t > 0.0) {
						vec3 hitPoint = pointAlongRay(ray, t);
					if (hitData != NULL) {
						hitData->distance = t;
						hitData->position = hitPoint;
						hitData->normal = v3_normalise(v3_sub(hitPoint, center));
					}
					return true;
				}
			}
	return false;
}
