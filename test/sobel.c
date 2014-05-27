#include "cv.h"
#include "highgui.h"
void MySobel(IplImage* gray, IplImage* gradient);

int main(int argc, char * argv[])
{
	IplImage  * img = cvLoadImage(argv[1]);
	IplImage  * gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage * mysobel = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);  
	IplImage * cvsobel = cvCreateImage(cvGetSize(img), IPL_DEPTH_16S, 1);
	IplImage * cov = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);;

	cvNamedWindow("img");
	cvNamedWindow("gray");
	cvNamedWindow("mysobel");
//	cvNamedWindow("cvsobel");
	cvNamedWindow("cov");

	cvShowImage("img", img);

	cvCvtColor(img, gray, CV_BGR2GRAY);

	cvShowImage("gray", gray);

	MySobel(gray, mysobel);
	cvSobel(gray, cvsobel, 1, 0, 3);
	cvConvertScaleAbs(cvsobel, cov, 1, 0);
	cvShowImage("mysobel", mysobel);
//	cvShowImage("cvsobel", cvsobel);
	cvShowImage("cov", cov);

	cvWaitKey(0);
	return 0;
}




void MySobel(IplImage* gray, IplImage* gradient)
 {
    /* Sobel template
    a00 a01 a02
    a10 a11 a12
   a20 a21 a22
    */

    unsigned char a00, a01, a02, a20, a21, a22;
    unsigned char a10, a11, a12;

    for (int i=1; i<gray->height-1; ++i)
     {
      for (int j=1; j<gray->width-1; ++j)
         {
             CvScalar color = cvGet2D(gray, i, j);
             
             a00 = cvGet2D(gray, i-1, j-1).val[0];
             a01 = cvGet2D(gray, i-1, j).val[0];
             a02 = cvGet2D(gray, i-1, j+1).val[0];
 
             a10 = cvGet2D(gray, i, j-1).val[0];
             a11 = cvGet2D(gray, i, j).val[0];
             a12 = cvGet2D(gray, i, j+1).val[0];
 
             a20 = cvGet2D(gray, i+1, j-1).val[0];
             a21 = cvGet2D(gray, i+1, j).val[0];
             a22 = cvGet2D(gray, i+1, j+1).val[0];
             
             // x方向上的近似导数
             double ux = a20 * (1) + a21 * (2) + a22 * (1) 
                 + (a00 * (-1) + a01 * (-2) + a02 * (-1));
 
             // y方向上的近似导数
             double uy = a02 * (1) + a12 * (2) + a22 * (1)
                 + a00 * (-1) + a10 * (-2) + a20 * (-1);
 
            color.val[0] = ux;
 
             cvSet2D(gradient, i, j, color);
        }
     }
 }
