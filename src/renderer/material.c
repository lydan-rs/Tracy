#include "material.h"
#include "../utils//random.h"
#include <stdlib.h>


vec3 randomUnitVector() {
	vec3 direction;
	while (true) {
		direction = (vec3){frandRange(-1.0, 1.0), frandRange(-1.0, 1.0), frandRange(-1.0, 1.0)};
		float sqrlen = v3_mag_sqrd(direction);
		if (1E-6 < sqrlen && sqrlen <= 1.0) {
			direction = v3_normalise(direction);
			return direction;
		}
	}
};



vec3 randomDiffusion(vec3 normal) {
	vec3 randDir = randomUnitVector();
	if (v3_dot(randDir, normal) > 0.0) {
		return randDir;
	} else {
		return v3_scale(randDir, -1.0);
	}
}

vec3 lambertianDiffusion(vec3 normal) {
	while (true) {
		vec3 direction = v3_add(normal, randomUnitVector());
		if (1E-6 < v3_mag_sqrd(direction)) {
			return v3_normalise(direction);
		}
	}
}



bool scatter(Photon* photon, HitRecord* hitRecord, SceneObject* objects, Material* materials) {
	if (photon == NULL
			|| hitRecord == NULL
			|| objects == NULL
			|| materials == NULL) {
		return false;
	}

	SceneObject object = objects[hitRecord->objectID];
	Material material = materials[object.materialID];

	// vec3 scatterDir = lambertianDiffusion(hitRecord->data.normal);
	vec3 scatterDir;
	if (material.isMetal) {
		scatterDir = v3_reflect(photon->ray.direction, hitRecord->data.normal);
	} else {
		scatterDir = lambertianDiffusion(hitRecord->data.normal);
	}

	photon->ray = (Ray){
		.origin = hitRecord->data.position,
		.direction = scatterDir
	};
	photon->attenuation = material.albedo;
	return true;
}
