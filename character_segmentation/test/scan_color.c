/*
   由于我们要实现的是从监控视频中获得车牌的信息，所以车牌占总图片的比例大小应该是差不多的，不会有非常大的差距，所以我们的思想是用一个比例为440：140左右的占图片比例大小为某一个较大又不是很大的窗口来筛选出这个区域，然后根据颜色以及窗口内颜色比例大小固定（车牌内文字大概在一个范围内的）来粗略定位车牌位置，
然后再进行精细定位
*/

#include "cv.h"
#include "highgui.h"

void draw_rectangle(IplImage * img);
void scan_img(IplImage *img);

int main(int argc, char * argv[])
{
	IplImage * original_img;
	original_img = cvLoadImage(argv[1]);
	cvNamedWindow("original_img");

	draw_rectangle(original_img);
	cvShowImage("original_img", original_img);

	cvWaitKey(0);
	return 0;
}

void draw_rectangle(IplImage * img)
{
	IplImage * draw_on_this_img;
	IplImage * gray_img;
	cvNamedWindow("draw_on_this_img");

	draw_on_this_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	draw_on_this_img = cvCloneImage(img);

	double scale =  3.14 ;
	int x = 320;
	int y = 200;

	int height = 40;

	int width = 1 * height * scale;
	printf("width:%d height:%d\n", img->width, img->height);
	cvRectangle(draw_on_this_img, cvPoint(x, y), cvPoint(x + width, y + height), cvScalar(201, 102, 62), 3, 4, 0);

	cvShowImage("draw_on_this_img", draw_on_this_img);
}

