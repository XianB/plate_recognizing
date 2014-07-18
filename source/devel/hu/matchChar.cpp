// matchChar.cpp : Defines the entry point for the console application.
//
#include "highgui.h"
#include "cv.h"
#include "cxcore.h"

/*
*
*����ת�� ��ע������ www.opencvchina.com
*
*
*/



/*
*��������:��ȡ����ͼ�� ������
*
*����: 
*srcIn : ��ͨ��ͼ��
*
*����:
*CvSeq* : �洢������Ϣ
*
*/
CvSeq* getImageContour(IplImage* srcIn)
{
	

	IplImage* src;
	src = cvCreateImage(cvGetSize(srcIn),8,1);
	
	//����ͼ�� 
	cvCopy(srcIn,src);

	//�����ռ�
	CvMemStorage* mem = cvCreateMemStorage(0);
	CvSeq* seq;

	if(!mem)
	{
		printf("mem is null");
	}

	//��ֵ��ͼ��
	cvThreshold(src,src,100,255,0);

	//����ͼ������
	cvFindContours(src,mem,&seq,sizeof(CvContour),CV_RETR_CCOMP);


	//�ͷ�ͼ��ռ�
	cvReleaseImage(&src);
	

	//����������Ϣ
	return seq;

}



int main(int argc, char* argv[])
{

	
	//����ͼ��1
	IplImage* src1 = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage * src2;
	int i = 0;
	char file[100];
	double result;
	
	//��ȡͼ��1������
	CvSeq* contour1;
	CvSeq* contour2;
	contour1 = getImageContour(src1);



	//����ͼ��2
	for (i = 0; i < 10; i++) {
		sprintf(file, "%d.png", i);
	src2 = cvLoadImage(file,CV_LOAD_IMAGE_GRAYSCALE);
	
	//��ȡͼ��2������
	contour2 = getImageContour(src2);


	//���бȶ�

	result = cvMatchShapes(contour1,contour2,1);

	//������
	printf("number %d: result is %f \n",i, result);
	}



	//�ͷ������洢�ռ�
	cvReleaseMemStorage(&contour1->storage);
	cvReleaseMemStorage(&contour2->storage);

	//�ͷ�ͼ��ռ�
	cvReleaseImage(&src1);
	cvReleaseImage(&src2);



	return 0;
}

