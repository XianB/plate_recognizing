// contourMatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "cvaux.h"



//www.opencvchina.com

int main(int argc, char* argv[])
{


	//产生一幅图像
	IplImage* src;
	src = cvCreateImage(cvSize(10,10),8,1);
	
	//图像初始值清零
	cvZero(src);

	//图像的前面5行5列赋值为255
	for(int yy=0;yy<5;yy++)
	{
		for(int xx=0;xx<5;xx++)
		{
			cvSetReal2D(src,yy,xx,255);
		}
	}

	double m00,m10,m01;

	//定义矩变量
	CvMoments moment;

	//计算原始矩和中心矩
	cvMoments(src,&moment,2);

	m00 = cvGetSpatialMoment(&moment,0,0);
	m10 = cvGetSpatialMoment(&moment,1,0);
	m01 = cvGetSpatialMoment(&moment,0,1);

	//计算质心坐标
	float x = (float)(m10/m00);
	float y = (float)(m01/m00);

	//定义hu矩变量
	CvHuMoments humoment;

	//计算hu矩
	cvGetHuMoments(&moment,&humoment);

	return 0;
}

