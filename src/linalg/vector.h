#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct {
	union {
		struct { int x, y; };
		struct { int r, g; };
		struct { int u, v; };
	};
} ivec2;

typedef struct {
	union {
		struct { float x, y; };
		struct { float r, g; };
		struct { float u, v; };
	};
} vec2;

typedef struct {
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float u, v, s; };
	};
} vec3;



ivec2  iv2_add (ivec2 a, ivec2 b);
ivec2  iv2_sub (ivec2 a, ivec2 b);
float  iv2_mag (ivec2 v);
ivec2  iv2_scale      (ivec2 v, float t);
float  iv2_mag_sqrd   (ivec2 v);
ivec2  iv2_normalise  (ivec2 v);
bool   iv2_contains   (ivec2 v, int x);
bool   iv2_surrounds  (ivec2 v, int x);
int    iv2_clamp      (ivec2 v, int x);
int    iv2_difference (ivec2 v);
ivec2  iv2_order      (ivec2 v);
ivec2  iv2_interval   (int min, int max);

vec2   v2_add (vec2 a, vec2 b);
vec2   v2_sub (vec2 a, vec2 b);
float  v2_mag (vec2 v);
vec2   v2_scale     (vec2 v, float t);
float  v2_mag_sqrd  (vec2 v);
vec2   v2_normalise (vec2 v);
bool   v2_contains  (vec2 v, float x);
bool   v2_surrounds (vec2 v, float x);
float  v2_clamp     (vec2 v, float x);
float  v2_difference (vec2 v);
vec2   v2_order      (vec2 v);
vec2   v2_interval   (float min, float max);

float  v3_dot (vec3 a, vec3 b);
vec3   v3_add (vec3 a, vec3 b);
vec3   v3_sub (vec3 a, vec3 b);
float  v3_mag (vec3 v);
vec3   v3_scale      (vec3 v, float t);
float  v3_mag_sqrd   (vec3 v);
vec3   v3_normalise  (vec3 v);
vec3   v3_cross_prod (vec3 a, vec3 b);

#endif
