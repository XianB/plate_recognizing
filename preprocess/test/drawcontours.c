/*这个程序可以找出一个二值图像的轮廓并且描绘出这个图像的所有外接矩阵*/

#include "stdio.h"  
#include "cv.h"  
#include "highgui.h"  
#include <math.h>  


int main(int argc, char * argv[])  
{  
	IplImage *src = cvLoadImage(argv[1], 0);  
	IplImage *dsw = cvCreateImage(cvGetSize(src), 8, 1);  
	IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3);  
	CvMemStorage *storage = cvCreateMemStorage(0);  
	CvSeq *first_contour = NULL;  

	//turn the src image to a binary image  
	//cvThreshold(src, dsw, 125, 255, CV_THRESH_BINARY_INV);  
	cvThreshold(src, dsw, 100, 255, CV_THRESH_BINARY);  

	cvFindContours(dsw, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);  
	cvZero(dst);  
	int cnt = 0;  
	for(; first_contour != 0; first_contour = first_contour->h_next)  
	{  
		cnt++;  
		CvScalar color = CV_RGB(rand()&255, rand()&255, rand()&255);  
//		cvDrawContours(dst, first_contour, color, color, 0, 2, CV_FILLED, cvPoint(0, 0));  
		CvRect rect = cvBoundingRect(first_contour,0);
		cvRectangle(dst, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255, 0, 0), 1, 8, 0);
	}  

	printf("the num of contours : %d\n", cnt);  

	cvNamedWindow( "Source", 1 );  
	cvShowImage( "Source", src );  

	cvNamedWindow( "dsw", 1 );  
	cvShowImage( "dsw", dsw );  

	cvNamedWindow( "Components", 1 );  
	cvShowImage( "Components", dst );  

	cvReleaseMemStorage(&storage);  
	cvWaitKey(-1);  

	return 0;  
}  
