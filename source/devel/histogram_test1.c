#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <assert.h>
#include <stdbool.h>


bool histogram_analysing(CvHistogram *hist, int bins);
IplImage* DrawHistogram(CvHistogram* hist , float scaleX = 1 , float scaleY = 1)
{

	//获取直方图中极大值
	float histMax = 0;
	cvGetMinMaxHistValue(hist , 0 ,&histMax ,0 ,0);

	//创建图像 该图像用于显示直方图
	IplImage* imgHist = cvCreateImage(cvSize(100*scaleX , 30*scaleY) , 8 ,1);

	//图像置零
	cvZero(imgHist);

	//依次绘制直方图的bin
	for(int i=0;i<4;i++)
	{

	//获取直方图的值
	float histValue = cvQueryHistValue_1D(hist , i);
	float nextValue = cvQueryHistValue_1D(hist , i+1);



	//获取四边形的四个点的坐标  详情请观看视频的讲解
	CvPoint pt1 = cvPoint(      i*scaleX , 64*scaleY);
	CvPoint pt2 = cvPoint(  (i+1)*scaleX , 64*scaleY);
	CvPoint pt3 = cvPoint(  (i+1)*scaleX , (64 - (nextValue/histMax)*64) *scaleY );
	CvPoint pt4 = cvPoint (      i*scaleX , (64 - (histValue/histMax)*64) *scaleY );

	int numPts = 5;
	CvPoint pts[5];
	pts[0] = pt1;
	pts[1] = pt2;
	pts[2] = pt3;
	pts[3] = pt4;
	pts[4] = pt1;

	//填充四边形
	cvFillConvexPoly(imgHist , pts ,numPts , cvScalar(255));

	}


	return imgHist;

}


int main(int argc, char* argv[])
{

	//加载图像
	IplImage* src_rgb = cvLoadImage(argv[1]);
	assert(src_rgb != NULL);
	IplImage * src = cvCreateImage(cvGetSize(src_rgb), src_rgb->depth, src_rgb->nChannels);

	cvNamedWindow("sr");
	cvShowImage("sr" , src_rgb);
	cvCvtColor(src_rgb, src, CV_RGB2HSV);
            



	int dims = 1;  // 一维直方图
	int size =10; //bin的个数
	float range[] = {0, 255}; //取值范围
	float* ranges[] = {range};

	CvHistogram* hist;

	//创建直方图
	hist = cvCreateHist(dims , &size , CV_HIST_ARRAY ,  ranges , 1 );

	//清空直方图
	cvClearHist(hist);

	//给B  G  R 三个通道的图像分配空间
	IplImage* img_h = cvCreateImage(cvGetSize(src) , 8 ,1);
	IplImage* img_s = cvCreateImage(cvGetSize(src) , 8 ,1);
	IplImage* img_v = cvCreateImage(cvGetSize(src) , 8 ,1);

	//将图像src  分解成B   G   R 三个通道
	cvSplit(src , img_h , img_s , img_v , NULL);

	//计算R通道 直方图
	cvCalcHist(&img_h , hist , 0 , 0 );

	//绘制R通道直方图  hist_h
	IplImage* hist_h = DrawHistogram(hist);

	if (histogram_analysing(hist,10) == true) {
		printf("It must be a plate!\n");
	} else
		printf("I don't think it is a plate\n");

	//将R通道的直方图数据清空
	cvClearHist(hist);


	cvNamedWindow("h");


	cvShowImage("h"  , hist_h);

	char filename_h[20];


	sprintf(filename_h, "%c_h.bmp", argv[1][24]);


	cvSaveImage(filename_h, hist_h);


	cvWaitKey(0);


	//释放资源

	//(1) 释放源图像
	cvReleaseImage(&src);

	//(2) 释放三个通道的图像
	cvReleaseImage(&img_h);


	//(3)释放三个通道直方图

	cvReleaseImage(&hist_h);

	//(4)释放直方图空间
	cvReleaseHist(&hist);

	return 0;
}

bool histogram_analysing(CvHistogram *hist, int bins)
{
	int i = 0;
	int sum = 0;
	double bin0 = -1;
	double bin1 = -1;
	int *ar = (int *)malloc(sizeof(int) * (bins + 1));
	for (i = 0; i < bins; i++) {
		float histValue = cvQueryHistValue_1D(hist , i);
		ar[i] = (int)histValue;
		sum += ar[i];
//		printf("%d\n", ar[i]);
	}

		bin0 = 1.0 * ar[0] / sum;
		bin1 = 1.0 * ar[1] / sum;

		printf("%lf %lf\n", bin0, bin1);

	if (bin0 > 0.75)
		return true;
	else 
		return false;
}


