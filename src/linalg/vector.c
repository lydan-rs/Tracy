#include "vector.h"
#include <math.h>

int imax(int a, int b) { return a>b ? a : b; }
int imin(int a, int b) { return a<b ? a : b; }

/*
 *  Integer Vector 2
 */

int iv2_dot(ivec2 a, ivec2 b) {
	return (a.x*b.x) + (a.y*b.y);
}


ivec2 iv2_add(ivec2 a, ivec2 b) {
	return (ivec2){
		.x = a.x+b.x,
		.y = a.y+b.y,
	};
}

ivec2 iv2_sub(ivec2 a, ivec2 b) {
	return (ivec2){
		.x = a.x-b.x,
		.y = a.y-b.y,
	};
}

ivec2 iv2_scale(ivec2 v, float scalar) {
	return (ivec2){
		.x = v.x*scalar,
		.y = v.y*scalar,
	};
}

float iv2_mag_sqrd(ivec2 v) {
	return (float)iv2_dot(v, v);
}

float iv2_mag(ivec2 v) {
	return sqrtf(iv2_mag_sqrd(v));
}

ivec2 iv2_normalise(ivec2 v) {
	const float mag = iv2_mag(v);
	return (ivec2){
		.x = v.x/mag,
		.y = v.y/mag,
	};
}

bool   iv2_contains   (ivec2 v, int x) { return imin(v.x, v.y) <= x && x <= imax(v.x, v.y); }
bool   iv2_surrounds  (ivec2 v, int x) { return imin(v.x, v.y) <  x && x <  imax(v.x, v.y); }
int    iv2_clamp      (ivec2 v, int x) { return imin(imax(x, v.x), v.y); }
int    iv2_difference (ivec2 v) { return v.y - v.x; }
ivec2  iv2_order      (ivec2 v) { return (ivec2){imin(v.x, v.y), imax(v.x, v.y)}; }
ivec2  iv2_interval   (int min, int max) { return (ivec2){imin(min, max), imax(min, max)}; }

/*
 *  Vector 2
 */

float v2_dot(vec2 a, vec2 b) {
	return (a.x*b.x) + (a.y*b.y);
}


vec2 v2_add(vec2 a, vec2 b) {
	return (vec2){
		.x = a.x+b.x,
		.y = a.y+b.y,
	};
}

vec2 v2_sub(vec2 a, vec2 b) {
	return (vec2){
		.x = a.x-b.x,
		.y = a.y-b.y,
	};
}

vec2 v2_scale(vec2 v, float scalar) {
	return (vec2){
		.x = v.x*scalar,
		.y = v.y*scalar,
	};
}

float v2_mag_sqrd(vec2 v) {
	return v2_dot(v, v);
}

float v2_mag(vec2 v) {
	return sqrtf(v2_mag_sqrd(v));
}

vec2 v2_normalise(vec2 v) {
	const float mag = v2_mag(v);
	return (vec2){
		.x = v.x/mag,
		.y = v.y/mag,
	};
}

bool   v2_contains  (vec2 v, float x) { return fmin(v.x, v.y) <= x && x <= fmin(v.x, v.y); }
bool   v2_surrounds (vec2 v, float x) { return fmin(v.x, v.y) <  x && x <  fmin(v.x, v.y); }
float  v2_clamp     (vec2 v, float x) { return fmin(fmax(x, v.x), v.y); }
float  v2_difference (vec2 v) { return v.y - v.x; }
vec2   v2_order      (vec2 v) { return (vec2){fmin(v.x, v.y), fmax(v.x, v.y)}; }
vec2   v2_interval   (float min, float max) { return (vec2){fmin(min, max), fmax(min, max)}; }


/*
 * Vector 3
 */

float v3_dot(vec3 a, vec3 b) {
	return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}


vec3 v3_add(vec3 a, vec3 b) {
	return (vec3){
		.x = a.x+b.x,
		.y = a.y+b.y,
		.z = a.z+b.z
	};
}

vec3 v3_sub(vec3 a, vec3 b) {
	return (vec3){
		.x = a.x-b.x,
		.y = a.y-b.y,
		.z = a.z-b.z
	};
}

vec3 v3_mult(vec3 a, vec3 b) {
	return (vec3){
		.x = a.x*b.x,
		.y = a.y*b.y,
		.z = a.z*b.z
	};
}

vec3 v3_scale(vec3 v, float scalar) {
	return (vec3){
		.x = v.x*scalar,
		.y = v.y*scalar,
		.z = v.z*scalar
	};
}

float v3_mag_sqrd(vec3 v) {
	return v3_dot(v, v);
}

float v3_mag(vec3 v) {
	return sqrtf(v3_mag_sqrd(v));
}

vec3 v3_normalise(vec3 v) {
	const float mag = v3_mag(v);
	return (vec3){
		.x = v.x/mag,
		.y = v.y/mag,
		.z = v.z/mag
	};
}

vec3 v3_cross_prod(vec3 a, vec3 b) {
	return (vec3){
		.x = (a.y*b.z)-(a.z*b.y),
		.y = (a.z*b.x)-(a.x*b.z),
		.z = (a.x*b.y)-(a.y-b.x)
	};
}

vec3 v3_reflect(vec3 incoming, vec3 normal) {
	return v3_sub(incoming, v3_scale(normal, 2*v3_dot(incoming, normal)));
}
