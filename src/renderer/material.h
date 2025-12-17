#ifndef MATERIAL_H
#define MATERIAL_H

#include "../linalg/vector.h"
#include "../linalg/ray.h"
#include "scene_object.h"

typedef struct {
	// Shader
	vec3 albedo;
	bool isMetal;
	// float roughness;
	// metalness
	unsigned int id;
} Material;

typedef struct {
	Ray  ray;
	// vec3 color;
	vec3 attenuation;
} Photon;

// TODO: Add errors
bool scatter(Photon* photon, HitRecord* hitRecord, SceneObject* objects, Material* materials);

#endif
