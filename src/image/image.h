#ifndef IMAGE_H
#define IMAGE_H

#include "../linalg/vector.h"

typedef enum {
	SUCCESS,
	FILE_IO,
	INDEX_OUT_OF_BOUNDS,
} IERR_CODE;

typedef struct {
	unsigned int height;
	unsigned int width;
	float aspect;
	vec3* data;
} Image;

Image*  createImage (unsigned int height, unsigned int width);
void    freeImage   (Image* image);

void  setPixel (Image* image, unsigned int x, unsigned int y, vec3 pixel);
vec3  getPixel (Image* image, unsigned int x, unsigned int y);


void  writePPM (Image* image, const char* path);

#endif
