#include "vec3.h"
#include <math.h>

float dotV3(Vec3 a, Vec3 b) {
	return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}


Vec3 addV3(Vec3 a, Vec3 b) {
	return (Vec3){
		.x = a.x+b.x,
		.y = a.y+b.y,
		.z = a.z+b.z
	};
}

Vec3 subtractV3(Vec3 a, Vec3 b) {
	return (Vec3){
		.x = a.x-b.x,
		.y = a.y-b.y,
		.z = a.z-b.z
	};
}

Vec3 scaleV3(Vec3 v, float scalar) {
	return (Vec3){
		.x = v.x*scalar,
		.y = v.y*scalar,
		.z = v.z*scalar
	};
}

float magSqrdV3(Vec3 v) {
	return dotV3(v, v);
}

float magnitudeV3(Vec3 v) {
	return sqrtf(magSqrdV3(v));
}

Vec3 normaliseV3(Vec3 v) {
	const float mag = magnitudeV3(v);
	return (Vec3){
		.x = v.x/mag,
		.y = v.y/mag,
		.z = v.z/mag
	};
}

Vec3 crossProdV3(Vec3 a, Vec3 b) {
	return (Vec3){
		.x = (a.y*b.z)-(a.z*b.y),
		.y = (a.z*b.x)-(a.x*b.z),
		.z = (a.x*b.y)-(a.y-b.x)
	};
}

