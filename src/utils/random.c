#include "random.h"
#include <stdlib.h>

float frand01() {
	return (float)rand() / RAND_MAX;
}

// TODO: Determine what happens when min is greater than max.
float frandRange(float min, float max) {
	float r = frand01();
	if (max < min) {
		float temp = min;
		min = max;
		max = temp;
	}
	return min + (max-min)*r;
}
