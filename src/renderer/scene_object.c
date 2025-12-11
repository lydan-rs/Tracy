#include "scene_object.h"


bool testIntersection(SceneObject* object, Ray ray, Intersection* intersection){
			if (object == NULL) {
				// TODO: Return Error
				return false;
			}

			// if (intersection == NULL) {
			// 	//TODO: Return Error
			// 	return false;
			// }

			switch (object->type) {
				case SPHERE:
					{
						vec3 hitPoint = {0,0,0};
						vec3 hitNormal = {0,0,0};
						bool intersects = intersectRaySphere(ray, object->position, 1.0, &hitPoint, &hitNormal);
						if (intersects) {
							intersection->hitPoint = hitPoint;
							intersection->hitNormal = hitNormal;
							intersection->objectID = 0; // TODO: Fix when Object IDs are added.
							intersection->primID = 0; // Sphere's only have one prim
						}
						return intersects;
					}
				default:
					// TODO: Return Error
					return false;
			}
}
