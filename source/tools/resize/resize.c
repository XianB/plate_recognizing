#include "cv.h"
#include "highgui.h"

int main(int argc, char * argv[])
{
	IplImage * src_img = cvLoadImage(argv[1], -1);
	if (src_img == NULL) {
		fprintf(stderr, "can not open file %s.\n", argv[1]);
		exit(-1);
	}
	IplImage * dst_img = cvCreateImage(cvSize(src_img->width * 5, src_img->height * 5), src_img->depth, src_img->nChannels);
	cvResize(src_img, dst_img);

	cvNamedWindow("img", 1);
	cvShowImage("img", dst_img);
	cvWaitKey(0);
	return 0;
}
