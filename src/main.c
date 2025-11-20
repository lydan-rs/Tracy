#include "stdio.h"

const int IMG_WIDTH = 256;
const int IMG_HEIGHT = 256;

int main() {
	printf("Hello Bec!\n");

	// int buffer[IMG_HEIGHT][IMG_WIDTH];

	FILE* file = fopen("output.ppm", "w");
	if (file == NULL) {
		printf("Cannot open output.ppm");
		return 1;
	}

	printf("Writing Image...\n");

	fprintf(file, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);

	for (int r = 0; r < IMG_HEIGHT; r++) {
		printf("\r%d/%d scanlines complete. %d remaning.", r+1, IMG_HEIGHT, (IMG_HEIGHT-r-1));
		for (int c = 0; c < IMG_WIDTH; c++) {
			float red = (float)c / (IMG_HEIGHT-1);
			float green = (float)r / (IMG_WIDTH-1);
			float blue = 0.0f;

			int iRed = (int)(255.66 * red);
			int iGreen = (int)(255.66 * green);
			int iBlue = (int)(255.66 * blue);

			fprintf(file, "%d %d %d\n", iRed, iGreen, iBlue);
		}
	}
	printf("\n");

	fclose(file);

	printf("Image finished! YAAAY!!!\n");

}
