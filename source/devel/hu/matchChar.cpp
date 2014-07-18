// matchChar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "cxcore.h"

/*
*
*如若转载 请注明出自 www.opencvchina.com
*
*
*/



/*
*函数功能:获取输入图像 的轮廓
*
*输入: 
*srcIn : 单通道图像
*
*返回:
*CvSeq* : 存储轮廓信息
*
*/
CvSeq* getImageContour(IplImage* srcIn)
{
	

	IplImage* src;
	src = cvCreateImage(cvGetSize(srcIn),8,1);
	
	//拷贝图像 
	cvCopy(srcIn,src);

	//创建空间
	CvMemStorage* mem = cvCreateMemStorage(0);
	CvSeq* seq;

	if(!mem)
	{
		printf("mem is null");
	}

	//二值化图像
	cvThreshold(src,src,100,255,0);

	//计算图像轮廓
	cvFindContours(src,mem,&seq,sizeof(CvContour),CV_RETR_CCOMP);


	//释放图像空间
	cvReleaseImage(&src);
	

	//返回轮廓信息
	return seq;

}



int main(int argc, char* argv[])
{

	
	//加载图像1
	IplImage* src1 = cvLoadImage("./images/2.png",CV_LOAD_IMAGE_GRAYSCALE);
	
	//获取图像1的轮廓
	CvSeq* contour1;
	contour1 = getImageContour(src1);



	//加载图像2
	IplImage* src2 = cvLoadImage("./images/3.png",CV_LOAD_IMAGE_GRAYSCALE);
	
	//获取图像2的轮廓
	CvSeq* contour2;
	contour2 = getImageContour(src2);


	//进行比对
	double result;

	result = cvMatchShapes(contour1,contour2,1);

	//输出结果
	printf("result is %f \n",result);



	//释放轮廓存储空间
	cvReleaseMemStorage(&contour1->storage);
	cvReleaseMemStorage(&contour2->storage);

	//释放图像空间
	cvReleaseImage(&src1);
	cvReleaseImage(&src2);



	return 0;
}

