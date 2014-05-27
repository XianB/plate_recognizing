#include "cv.h"
#include "highgui.h"

void median_filtering(IplImage * img);
void show_image_value_one(IplImage * img);
void create_image(IplImage * img);
void get_image_max_min_value(IplImage * img, int * max, int * min);

IplImage * gray_strecth(IplImage * src_img, IplImage * dst_img, int exp_max, int exp_min);

int main(int argc, char * argv[])
{
	IplImage * test_img;
	IplImage * gray_img;
	IplImage * test_img_gray_st;
	cvNamedWindow("test_img");
	cvNamedWindow("gray_img");
	cvNamedWindow("test_img_gray_st");
	
	test_img = cvLoadImage(argv[1]);

	gray_img = cvCreateImage(cvGetSize(test_img), IPL_DEPTH_8U, 1);
	test_img_gray_st = cvCreateImage(cvGetSize(test_img), IPL_DEPTH_8U, 1);

	cvCvtColor(test_img, gray_img, CV_BGR2GRAY);
//	test_img = cvCreateImage(cvSize(5, 5), IPL_DEPTH_8U, 1);

//	create_image(test_img);

//	show_image_value_one(test_img);

	gray_strecth(gray_img, test_img_gray_st, 255, 0);

	cvShowImage("test_img", test_img);
	cvShowImage("gray_img", gray_img);
	cvShowImage("test_img_gray_st", test_img_gray_st);

	cvWaitKey(0);

//	show_image_value_one(test_img_gray_st);

	return 0;
}

IplImage * gray_strecth(IplImage * src_img, IplImage* dst_img, int exp_max, int exp_min)
{
	int ori_max, ori_min;
	get_image_max_min_value(src_img, &ori_max, &ori_min);

	for (int i = 0; i < src_img->height; i++) {
		unsigned char * srow = (unsigned char *)(src_img->imageData + i * src_img->widthStep);
		unsigned char * drow = (unsigned char *)(dst_img->imageData + i * dst_img->widthStep);
		for (int j = 0; j < src_img->width; j++){
			if (srow[j] < ori_min)
			  drow[j] = (exp_min / ori_min) * srow[j];
			else if (srow[j] >= ori_min && srow[j] <= ori_max)
			  drow[j] = (exp_max - exp_min) / (ori_max - ori_min) * (srow[j] - ori_min) + exp_min;
			else if (srow[j] > exp_max)
			  drow[j] = (255 - exp_max) / (255 - ori_max) * (srow[j] - ori_max) + exp_max;
		}
	
	}
}

void get_image_max_min_value(IplImage * img, int * max, int * min)
{
	int max_temp = -255, min_temp = 400;

	for (int i = 0; i < img->height; i ++) {

		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);

		for (int j; j < img->width; j ++) {

			if (prow[j] >= max_temp)	
			  max_temp = prow[j];

			if (prow[j] <= min_temp)
			  min_temp = prow[j];
		}
	}

	*max = max_temp;
	*min = min_temp;
}
		

void show_image_value_one(IplImage * img)
{
	int i, j;
	FILE * fp_value;

	fp_value = fopen("value.txt", "a");

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width; j++) {
			fprintf(fp_value, "%d ", prow[j]);
		}
		fprintf(fp_value, "\n");

	}

	fprintf(fp_value, "\n");
	fclose(fp_value);
}

void create_image(IplImage * img)
{
	FILE * fp_test = fopen("test.txt", "r");

	int i, j;
	int count;
	fscanf(fp_test, "%d", &count);;

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
		for (j = 0; j < img->width; j++){
			prow[j] = count;	
			fscanf(fp_test, "%d", &count);;
		}

	}
}







