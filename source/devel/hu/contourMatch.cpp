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


	//����һ��ͼ��
	IplImage* src;
	src = cvCreateImage(cvSize(10,10),8,1);
	
	//ͼ���ʼֵ����
	cvZero(src);

	//ͼ���ǰ��5��5�и�ֵΪ255
	for(int yy=0;yy<5;yy++)
	{
		for(int xx=0;xx<5;xx++)
		{
			cvSetReal2D(src,yy,xx,255);
		}
	}

	double m00,m10,m01;

	//����ر���
	CvMoments moment;

	//����ԭʼ�غ����ľ�
	cvMoments(src,&moment,2);

	m00 = cvGetSpatialMoment(&moment,0,0);
	m10 = cvGetSpatialMoment(&moment,1,0);
	m01 = cvGetSpatialMoment(&moment,0,1);

	//������������
	float x = (float)(m10/m00);
	float y = (float)(m01/m00);

	//����hu�ر���
	CvHuMoments humoment;

	//����hu��
	cvGetHuMoments(&moment,&humoment);

	return 0;
}

