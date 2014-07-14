#include "cv.h"
#include "highgui.h"

int main(int argc, char * argv[])
{
	IplImage * img_to_resize = cvLoadImage(argv[1]);

	float scale = 5;
	CvSize dst_size;

	dst_size.width = (int)(img_to_resize->width * scale);
	dst_size.height = (int)(img_to_resize->height * scale);

	IplImage * img_after_resize = cvCreateImage(dst_size, img_to_resize->depth, img_to_resize->nChannels);

	cvNamedWindow("img_to_resize");
	cvNamedWindow("img_after_resize");

	cvResize(img_to_resize, img_after_resize);

	cvShowImage("img_to_resize", img_to_resize);
	cvShowImage("img_after_resize", img_after_resize);
	cvSaveImage("img_after_resize.bmp",img_after_resize);

	cvWaitKey(0);

	return 0;
}
