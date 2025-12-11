#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <stdlib.h>
#include "../linalg/ray.h"

typedef enum {
	SPHERE,
	// BOX,
	// PLANE,
	// GRID,
	// CONE,
	// CYLINDER,
	// MESH
} ObjectType;

typedef struct {
	vec3 position;
	// Transforms
	ObjectType type;
	// ShaderType
	// Material
	// Mesh Data
	// BBox
} SceneObject;

typedef struct {
	vec3 hitPoint;
	vec3 hitNormal;
	int  objectID;
	int  primID;
} Intersection;

bool testIntersection(SceneObject* object, Ray ray, Intersection* intersection);

#endif
