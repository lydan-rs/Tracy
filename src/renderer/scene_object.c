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
						bool intersects = intersectRaySphere(ray, object->position, object->scale, &hitData);
						if (intersects && intersection != NULL) {
							intersection->data = hitData;
							intersection->objectID = object->id;
							intersection->primID = 0; // Sphere's only have one prim
						}
						return intersects;
					}
				default:
					// TODO: Return Error
					return false;
			}
}
