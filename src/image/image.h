#ifndef IMAGE_H
#define IMAGE_H

#include "../linalg/vec3.h"

typedef enum {
	SUCCESS,
	FILE_IO,
	INDEX_OUT_OF_BOUNDS,
} IERR_CODE;

typedef struct {
	unsigned int height;
	unsigned int width;
	float aspect;
	Vec3* data;
} Image;

Image*  createImage (unsigned int height, unsigned int width);
void    freeImage   (Image* image);

void  setPixel (Image* image, unsigned int x, unsigned int y, Vec3 pixel);
Vec3  getPixel (Image* image, unsigned int x, unsigned int y);


void  writePPM (Image* image, const char* path);

#endif
