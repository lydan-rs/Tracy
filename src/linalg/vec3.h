#ifndef VEC3_H
#define VEC3_H

typedef struct {
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float u, v, s; };
	};
} Vec3;

float dotV3(Vec3 a, Vec3 b);

#endif
