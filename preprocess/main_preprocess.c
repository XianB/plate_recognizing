/*这里没有用到滤波操作，因为滤波好像会把车牌信息弄不见*/
/*这个程序算是到目前为止功能比较完善的main函数了～2014.3.29*/

#include "cv.h"
#include "highgui.h"
#include <list>
using namespace std;
void show_image_value_one(IplImage * img);

void median_filtering(IplImage * img);
int compare(const void * num1, const void * num2);
void preprocess_the_image(IplImage * img_to_preprocess, IplImage * img_after_preprocess);
IplImage * gray_the_image(IplImage * color_img, IplImage * gray_img);
IplImage * gray_strecth(IplImage * src_img, IplImage * dst_img, int exp_max, int exp_min);
IplImage *  binarize_the_image(IplImage * img);
void Sobel_dir( IplImage* img, IplImage* dst);
void get_image_max_min_value(IplImage * img, int * max, int * min);
int  get_threshold_for_binarization(IplImage * img);
void clone_image(IplImage * src_img, IplImage * dst_img);//注意，此函数只能复制单通道图像，彩色图像不可以的！

int main(int argc, char * argv[])
{
	IplImage * img_to_preprocess;
	IplImage * img_after_preprocess;
	cvNamedWindow("img_to_preprocess");
	cvNamedWindow("img_after_preprocess");

	img_to_preprocess = cvLoadImage(argv[1]);
	img_after_preprocess = cvCreateImage(cvGetSize(img_to_preprocess), IPL_DEPTH_8U, 1);
	
	preprocess_the_image(img_to_preprocess, img_after_preprocess);
	cvSaveImage("img_after_preprocess.bmp", img_after_preprocess);

	cvShowImage("img_to_preprocess", img_to_preprocess);

	cvShowImage("img_after_preprocess", img_after_preprocess);

	cvWaitKey(0);

	return 0;

}


void preprocess_the_image(IplImage * img_to_preprocess, IplImage * img_after_preprocess)
{
	IplImage * gray_img;
	IplImage * img_after_filter;
	IplImage * img_after_stre;
	IplImage * img_after_sobel;
	IplImage * final_img;
#if 0
	cvNamedWindow("gray_img");
	cvNamedWindow("img_after_stre");
	cvNamedWindow("img_after_sobel");
	cvNamedWindow("img_after_filter");
#endif

//	cvNamedWindow("img_after_preprocess");
		
	gray_img = cvCreateImage(cvGetSize(img_to_preprocess), IPL_DEPTH_8U, 1);
	img_after_sobel = cvCreateImage(cvGetSize(img_to_preprocess), IPL_DEPTH_8U, 1);
	img_after_stre = cvCreateImage(cvGetSize(img_to_preprocess), IPL_DEPTH_8U, 1);
	img_after_filter = cvCreateImage(cvGetSize(img_to_preprocess), IPL_DEPTH_8U, 1);
	final_img = cvCreateImage(cvGetSize(img_to_preprocess), IPL_DEPTH_8U, 1);

	gray_the_image(img_to_preprocess, gray_img);
	//cvShowImage("gray_img", gray_img);

#if 0 //目的是看看中值滤波在哪里比较好，这里是第一个,在灰度化后，灰度拉伸前
	img_after_filter = cvCloneImage(gray_img);

	median_filtering(img_after_filter);
	cvShowImage("img_after_filter", img_after_filter);
#endif

//	img_after_stre = cvCloneImage(img_after_filter);
	img_after_stre = cvCloneImage(gray_img); //为了做测试，这里仅仅是复制灰度化后的图像
//	gray_strecth(img_after_filter, img_after_stre, 255, 0);
	gray_strecth(gray_img, img_after_stre, 255, 0);
	//cvShowImage("img_after_stre", img_after_stre);

#if 1 //这里是第二个地方，在灰度拉伸后，在sobel之前
	img_after_filter = cvCloneImage(img_after_stre);
	median_filtering(img_after_filter);
//	cvShowImage("img_after_filter", img_after_filter);
#endif

//	Sobel_dir(img_after_stre, img_after_sobel);
//	cvCanny(img_after_stre, img_after_sobel, 50, 150, 3);
	cvSobel(img_after_stre, img_after_sobel, 1, 0, 3);
	//cvShowImage("img_after_sobel", img_after_sobel);

	//cvThreshold(img_after_sobel, final_img, 0, 255, CV_THRESH_BINARY| CV_THRESH_OTSU);
	final_img = binarize_the_image(img_after_sobel);
//	img_after_preprocess = cvCloneImage(temp);
//	median_filtering(final_img);

#if 1 //进行膨胀腐蚀操作, 以及轮廓检测操作
	IplConvKernel* kernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);
	IplImage * erode_dilate = cvCreateImage(cvGetSize(final_img), IPL_DEPTH_8U, 1);

	cvDilate(final_img, erode_dilate, kernal, 5);//X方向膨胀连通数字

	cvErode(erode_dilate, erode_dilate, kernal, 4);//X方向腐蚀去除碎片

	cvDilate(erode_dilate, erode_dilate, kernal, 1);//X方向膨胀回复形态

	IplImage* copy = cvCloneImage(erode_dilate);
	//IplImage * dst = cvCreateImage(cvGetSize(erode_dilate), 8, 3);

	//cvNamedWindow("dst");

	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSeq* contours;

     cvFindContours(copy, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);    

	 //cvZero(dst);
	while(contours != NULL)
	{
//		list<struct CvRect> rects;
//		rects.push_back(cvBoundingRect(contours));
	//	CvScalar color = CV_RGB(255, 255, 255);
//		cvDrawContours(dst, contours, color, color, 0, 2, CV_FILLED, cvPoint(0, 0));
	//	cvDrawContours(copy, contours, color, color, 0, 1, 0, cvPoint(0, 0));


		CvRect rect = cvBoundingRect( contours, 0 );
//		cvRectangle( erode_dilate, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(0,0,0), 0 );
		cvRectangle(erode_dilate, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255, 21, 123), 1, 8, 0);

		contours= contours->h_next;
	}

//	cvShowImage("dst", dst);
#endif

	clone_image(erode_dilate, img_after_preprocess);
	//clone_image(final_img, img_after_preprocess);

//	median_filtering(img_after_preprocess);
//	show_image_value_one(img_after_preprocess);
//	cvShowImage("img_after_preprocess", img_after_preprocess);
}

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



void Sobel_dir( IplImage* img, IplImage* dst)
{
	int dx[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
	int dy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

	int step = img->widthStep/sizeof(uchar);
	unsigned char* data = (unsigned char *)img->imageData;
	unsigned char* data_dst = (unsigned char *)dst->imageData;
	
	int s;
	for (int i=1; i < img->height-2; i++)
		for (int j=1; j < img->width-2; j++)
		{
			// apply kernel in X direction
			int sum_x=0;
			for(int m=-1; m<=1; m++)
				for(int n=-1; n<=1; n++)
				{
					s=data[(i+m)*step+j+n]; // get the (i,j) pixel value
					sum_x+=s*dx[m+1][n+1];
				}
			// apply kernel in Y direction
			int sum_y=0;
			for(int m=-1; m<=1; m++)
				for(int n=-1; n<=1; n++)
				{
					s=data[(i+m)*step+j+n]; // get the (i,j) pixel value
					sum_y+=s*dy[m+1][n+1];
				}
			int sum = sum_x;
//			int sum=abs(sum_x)+abs(sum_y);
//			int sum=sqrt(abs(sum_x) * abs(sum_x) + abs(sum_y) * abs(sum_y));
			if (sum>255)
				sum=255;
			data_dst[i*step+j]=sum; // set the (i,j) pixel value
		}
}

IplImage *  binarize_the_image(IplImage * img)
{
//	int threshold = get_threshold_for_binarization(img);
//	int threshold = 254;
	int i, j;

	int threshold = get_threshold_for_binarization(img);
	
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
//		puts("\n");
	}
}


int compare(const void * num1, const void * num2)
{
	return *((int *)num1) - *(int *)num2;
}

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

/*注意，这个函数只能够复制单通道图像，彩色图像是不可以的*/
void clone_image(IplImage * src_img, IplImage * dst_img)
{
	for (int i = 0; i < src_img->height; i++) {

		unsigned char * srow = (unsigned char *)(src_img->imageData + i * src_img->widthStep);
		unsigned char * drow = (unsigned char *)(dst_img->imageData + i * dst_img->widthStep);

		for (int j = 0; j < src_img->width; j++) {
			drow[j] = srow[j];
		}
	}
}

