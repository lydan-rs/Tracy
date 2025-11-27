#include "vec3.h"

float dotV3(Vec3 a, Vec3 b) {
	return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}
