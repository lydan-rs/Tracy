#ifndef VEC3_H
#define VEC3_H

typedef struct {
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float u, v, s; };
	};
} Vec3;

float  dotV3       (Vec3 a, Vec3 b);
Vec3   addV3       (Vec3 a, Vec3 b);
Vec3   subtract    (Vec3 a, Vec3 b);
Vec3   scaleV3     (float scalar, Vec3 v);
float  magSqrdV3   (Vec3 v);
float  magnitudeV3 (Vec3 v);
Vec3   normalise   (Vec3 v);
Vec3   crossProdV3 (Vec3 a, Vec3 b);

#endif
