#ifndef IMAGE_H
#define IMAGE_H

#include "../linalg/vec3.h"

typedef enum {
	SUCCESS,
	FILE_IO,
} IMG_ERROR_CODES;

typedef struct {
	unsigned int width;
	unsigned int height;
	Vec3* data;
} Image;

Image* createImage(unsigned int width, unsigned int height);
void freeImage(Image* image);

IMG_ERROR_CODES writePPM(Image* image, const char* path);

#endif
