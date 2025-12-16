#include "scene_object.h"
#include <stdlib.h>


bool testIntersection(SceneObject* object, Ray ray, HitRecord* intersection){
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
						HitData hitData;
						// TODO: Unhardcode radius
						bool intersects = intersectRaySphere(ray, object->position, 1.0, &hitData);
						if (intersects && intersection != NULL) {
							intersection->data = hitData;
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
