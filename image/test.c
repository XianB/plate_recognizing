#include "cv.h"
#include "highgui.h"

int main(int argc, char * argv[])
{
	IplImage * img = cvLoadImage(argv[1]);
	IplImage * grayScale;
	IplImage * sobel;
	IplImage * threshold;
	IplImage * erode_dilate;

	erode_dilate = cvCreateImage(cvGetSize(img), img->depth, 1);
	grayScale = cvCreateImage(cvGetSize(img), img->depth, 1);
	threshold = cvCreateImage(cvGetSize(img), img->depth, 1);
	
	cvNamedWindow("original");
//	cvNamedWindow("grayScale");
//	cvNamedWindow("sobel");
//	cvNamedWindow("threshold");
	cvNamedWindow("erode_dilate");
	cvShowImage("original", img);

	cvCvtColor(img, grayScale,CV_BGR2GRAY );
	cvShowImage("grayScale", grayScale);

	sobel = cvCreateImage(cvGetSize(grayScale), IPL_DEPTH_16S,1);
	cvSobel(grayScale, sobel, 2, 0, 7);
	

	IplImage * temp = cvCreateImage(cvGetSize(sobel), IPL_DEPTH_8U, 1);
	cvConvertScale(sobel, temp, 0.00390625, 0);

	cvShowImage("sobel", temp);

	cvThreshold(temp, threshold, 0, 255, CV_THRESH_BINARY| CV_THRESH_OTSU);
//	cvAdaptiveThreshold( sobel, threshold, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 21);
	cvShowImage("threshold", threshold);


	IplConvKernel* kernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);

	cvDilate(threshold, erode_dilate, kernal, 2);

	cvErode(erode_dilate, erode_dilate, kernal, 4);

	cvDilate(erode_dilate, erode_dilate, kernal, 2);

#if 0
	kernal = cvCreateStructuringElementEx(1, 3, 0, 1, CV_SHAPE_RECT);

	cvErode(erode_dilate, erode_dilate, kernal, 1);

	cvDilate(erode_dilate, erode_dilate, kernal, 2);

#endif
	cvShowImage("erode_dilate", erode_dilate);
	cvWaitKey(0);

	return 0;


}
