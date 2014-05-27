/*This program show a picture whost value is all -1
 and we can know that the picture whose value is all -1 just like a image whose value is all 255
 */

#include "cv.h"
#include "highgui.h"

void create_minus_img(IplImage * img);

int main(int argc, char * argv[])
{
	IplImage * minus_img;

	minus_img = cvCreateImage(cvSize(100, 100), IPL_DEPTH_8U, 1);

	create_minus_img(minus_img);
	cvNamedWindow("minus_img");
	cvShowImage("minus_img", minus_img);

	cvWaitKey(0);
	return 0;


}

void create_minus_img(IplImage * img)
{
	int value = -1;

	for (int i = 0; i < img->height; i++) {

		signed char * pdata = (signed char *)(img->imageData + img->widthStep * i);

		for (int j = 0; j < img->width; j++) {
			pdata[j] = 180;
		}
	}
}
