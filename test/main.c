#include "cv.h"
#include "highgui.h"

IplImage * gray_the_image(IplImage * color_img, IplImage * gray_img);

void show_image_value_one(IplImage * img);
void show_image_value_three(IplImage * img);

int main(int argc, char * argv[])
{
	IplImage * img_to_recog;
	IplImage * gray_img;

	cvNamedWindow("img_to_recog");
	cvNamedWindow("gray_img");


	img_to_recog = cvLoadImage(argv[1]);
	gray_img = cvCreateImage(cvGetSize(img_to_recog), IPL_DEPTH_8U, 1);

	gray_the_image(img_to_recog, gray_img);

	cvShowImage("img_to_recog", img_to_recog);
	cvShowImage("gray_img", gray_img);

	cvWaitKey(0);

	puts("the gray image's value");
	show_image_value_one(gray_img);

	puts("the color image's value");
	
	show_image_value_three(img_to_recog);



#if 0
	puts("the b image's value");
	show_image_value(gray_img);

	puts("the g image's value");
	show_image_value(gray_img);

	puts("the r image's value");
	show_image_value(gray_img);

#endif

	return 0;
}




//图像灰度化函数
IplImage * gray_the_image(IplImage * color_img, IplImage * gray_img)
{
	int i, j;
	for (i = 0; i < color_img->height; i ++) {
		unsigned char * prow = (unsigned char *)(color_img->imageData + i * color_img->widthStep);
	
		unsigned char * grow = (unsigned char *) (gray_img->imageData + i * gray_img->widthStep);
		for (j = 0; j < color_img->width; j++) {
			grow[j]= prow[3 * j + 0] * 0.3 + prow[3 * j + 1] * 0.59 + prow[3 * j + 2] * 0.11;
		}

	}
	return gray_img;
}



//显示单通道图像的值
void show_image_value_one(IplImage * img)
{
	int i, j;
	for (i = 0; i < img->height/100; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width/100; j++) {
			printf(" %d ", prow[j]);
		}
		puts("\n");

	}

}

void show_image_value_three(IplImage * img)
{
	int i, j;
	for (i = 0; i < img->height/100; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width/100; j++) {
			printf("( %d  %d %d )", prow[j * 3 + 0], prow[j * 3 + 1], prow[j * 3 + 2]);
		}
		puts("\n");

	}

}
