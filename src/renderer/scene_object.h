#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

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
	HitData data;
	int  objectID;
	int  primID;
} HitRecord;

bool testIntersection(SceneObject* object, Ray ray, HitRecord* intersection);

#endif
