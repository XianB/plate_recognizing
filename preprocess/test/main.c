#include "cv.h"
#include "highgui.h"

IplImage * gray_the_image(IplImage * color_img, IplImage * gray_img);
void median_filtering(IplImage * img);
void show_image_value_one(IplImage * img);
void create_image(IplImage * img);
int compare(const void * num1, const void * num2);


void show_image_value_one(IplImage * img);
void show_image_value_three(IplImage * img);
int  get_threshold_for_binarization(IplImage * img);

IplImage * binarize_the_image(IplImage * img);

int main(int argc, char * argv[])
{
	IplImage * img_to_recog;
	IplImage * gray_img;
	IplImage * binary_img;
	IplImage * median_filter_img;

	cvNamedWindow("img_to_recog");
	cvNamedWindow("gray_img");
	cvNamedWindow("binary_img");
	cvNamedWindow("median_filter_img");


	img_to_recog = cvLoadImage(argv[1]);

	gray_img = cvCreateImage(cvGetSize(img_to_recog), IPL_DEPTH_8U, 1);
//	binary_img = cvCreateImage(cvGetSize(img_to_recog), IPL_DEPTH_8U, 1);

	gray_the_image(img_to_recog, gray_img);
	binary_img = binarize_the_image(gray_img);

	median_filter_img = cvCloneImage(binary_img);
	median_filtering(median_filter_img);

	cvShowImage("img_to_recog", img_to_recog);
	cvShowImage("gray_img", gray_img);
	cvShowImage("binary_img", binary_img);
	cvShowImage("median_filter_img", median_filter_img);

	cvWaitKey(0);

	puts("the gray image's value");
	show_image_value_one(gray_img);

//	puts("the color image's value");
	
//	show_image_value_three(img_to_recog);


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
			grow[j]= prow[3 * j + 0] * 0.114 + prow[3 * j + 1] * 0.587 + prow[3 * j + 2] * 0.3;
		}

	}
	return gray_img;
}



//显示单通道图像的值
void show_image_value_one(IplImage * img)
{
	int i, j;
	FILE * fp_value;

	fp_value = fopen("value.txt", "w");

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width; j++) {
			fprintf(fp_value, "%d ", prow[j]);
		}
		fprintf(fp_value, "\n");

	}

}

void show_image_value_three(IplImage * img)
{
	int i, j;
	for (i = 0; i < img->height/10; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width/10; j++) {
			printf("( %d  %d %d )", prow[j * 3 + 0], prow[j * 3 + 1], prow[j * 3 + 2]);
		}
		puts("\n");

	}

}


IplImage *  binarize_the_image(IplImage * img)
{
	int threshold = 120;
	int i, j;

//	threshold = get_threshold_for_binarization(img);
	
	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);

		for (j = 0; j < img->width; j++) {
			if (prow[j] > threshold)
			  prow[j] = 255;
			else
			  prow[j] = 0;
		}

	}

	return img;

}


//这里先使用自适应取阈值法
//事实显示自适应取阈值法并不是特别有效！

int  get_threshold_for_binarization(IplImage * img)
{
	int i, j;
	int threshold;

	int max = -1, min = 257;

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);

		for (j = 0; j < img->width; j++) {
			if (prow[j] > max)
			  max = prow[j];
			if (prow[j] < min)
			  min = prow[j];
		}

	}
	
	printf("the max/min number of this image is: %d %d\n ", max, min);

	threshold = max - (max - min) / 3;
	printf("the threshold is %d\n", threshold);
	return threshold;

}



void median_filtering(IplImage * img)
{

	int i, j;
	int a00, a01, a02;
	int a10, a11, a12;
	int a20, a21, a22;
	int a[9];


	for (i = 1; i < img->height - 1; i ++) {

		unsigned char * row0 = (unsigned char *)(img->imageData + (i - 1) * img->widthStep);
		
		unsigned char * row1 = (unsigned char *)(img->imageData + (i + 0) * img->widthStep);
		
		unsigned char * row2 = (unsigned char *)(img->imageData + (i + 1) * img->widthStep);


		for (j = 1; j < img->width - 1; j++) {
			a[0] = a00 = row0[j - 1];
			a[1] = a01 = row0[j + 0];
			a[2] = a02 = row0[j + 1];

			a[3] = a10 = row1[j - 1];
			a[4] = a11 = row1[j + 0];
			a[5] = a12 = row1[j + 1];

			a[6] = a20 = row2[j - 1];
			a[7] = a21 = row2[j + 0];
			a[8] = a22 = row2[j + 1];
	
//			printf("(%d %d %d)\n (%d %d %d)\n (%d %d %d)\n\n", a00, a01, a02, a10,a11,a12,a20,a21,a22);

			qsort(a, 9, sizeof(int), compare);
			row1[j] = a[4];
//			printf("%d\n",a[4]);
		}
		puts("\n");
	}
}

int compare(const void * num1, const void * num2)
{
	return *((int *)num1) - *(int *)num2;
}
