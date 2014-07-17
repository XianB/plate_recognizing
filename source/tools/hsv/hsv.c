#include "../../include/plate.h"

int main(int argc, char * argv[])
{
	IplImage * img_rgb = cvLoadImage(argv[1], -1);
	IplImage * img_hsv = NULL;
	IplImage * img_h = NULL;
	IplImage * img_s = NULL;
	IplImage * img_v = NULL;
	int i, j;

#if 1
	cvNamedWindow("rgb", 1);
	cvNamedWindow("hsv", 1);
	cvNamedWindow("h", 1);
	cvNamedWindow("s", 1);
	cvNamedWindow("v", 1);
#endif

	assert(img_rgb != NULL);
	assert(img_rgb->nChannels == 3);

	img_hsv = cvCreateImage(cvGetSize(img_rgb), IPL_DEPTH_8U, 3);
	img_h = cvCreateImage(cvGetSize(img_rgb), IPL_DEPTH_8U, 1);
	img_s = cvCreateImage(cvGetSize(img_rgb), IPL_DEPTH_8U, 1);
	img_v = cvCreateImage(cvGetSize(img_rgb), IPL_DEPTH_8U, 1);

	cvCvtColor(img_rgb, img_hsv, CV_RGB2HSV);
	cvSplit(img_hsv, img_h, img_s, img_v, NULL);

	for(i = 0; i < img_hsv->height; i++) {
		unsigned char * prowh = (unsigned char *)(img_h->imageData + i * img_h->widthStep);
		unsigned char * prows= (unsigned char *)(img_s->imageData + i * img_s->widthStep);
		unsigned char * prowv = (unsigned char *)(img_v->imageData + i * img_v->widthStep);

		for (j = 0; j < img_hsv->width; j++) {
			printf("h:%d s:%d v:%d\n", prowh[i], prows[i], prowv[i]);
		}
	}


#if 1
	cvShowImage("rgb", img_rgb);
	cvShowImage("hsv", img_hsv);
	cvShowImage("h", img_h);
	cvShowImage("s", img_s);
	cvShowImage("v", img_v);

	cvWaitKey(0);
#endif
	return 0;
}














