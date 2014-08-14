#include "../../include/plate.h"

#define SHAN

#ifdef LU
#define MAX_COUNT 51
#define PATH "../../../image/ln/plate/plate(%d).png"
#endif

#ifdef SHAN
#define MAX_COUNT 20
#define PATH "../../../image/plate_image/%d.png"
#endif

int main(int argc, char * argv[])
{
	IplImage * plate_img = NULL;
	char filename[100];
	int width = 0, height = 0;
	double scale;
	double min_scale = 10000;
	double max_scale = -10000;
	int i = 0;

	for (i = 1; i <= MAX_COUNT; i++) {
		sprintf(filename, PATH, i);
		if ((plate_img = cvLoadImage(filename, -1)) == NULL) {
			fprintf(stderr, "Can not open load image %s\n", filename);
			exit(-1);
		}
		printf("%d.png  width: %d height: %d scale: %lf\n", i, plate_img->width, plate_img->height, 1.0 * plate_img->width / plate_img->height);
		scale = 1.0 * plate_img->width /plate_img->height;

		width += plate_img->width;
		height += plate_img->height;


		if (scale > max_scale)
			max_scale = scale;

		if(scale < min_scale)
			min_scale = scale;
	}

	printf("\n\navg width: %d, avg %d, avg scale: %lf\n", (int)(width / MAX_COUNT), (int)(height / MAX_COUNT), (double) 1.0 * width / height);

	printf("\nmax_scale:%lf, min_scale:%lf\n", max_scale, min_scale);
	return 0;
}
