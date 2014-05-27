#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")
#define T 27
#define T1 2
#define S(image,x,y) ((uchar*)(image->imageData + image->widthStep*(y)))[(x)]		//S

int main(int argc, char * argv[])
{
	IplImage *src;
	IplImage *pImg8u=NULL;             //�Ҷ�ͼ
	IplImage *pImg8uSmooth=NULL;       //��˹�˲����ͼ
	IplImage *pImgCanny=NULL;          //��ֵ����ͼ
	IplImage *pImgHist=NULL;           //ֱ��ͼ
	int hist_size=155;
	float range_0[]={0,256};
	float *ranges[]={range_0};
	int i,j,bin_w;
	float max_value,min_value;
	int min_dx,max_dx;
	int row_start,row_end;//������¼���ƿ�ʼ��������
	int col_start,col_end;//������¼���ƿ�ʼ��������
	int count=0;//������¼�л��еİ׵����

	src=cvLoadImage(argv[1],-1);
	pImg8uSmooth=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	pImg8u=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	pImgCanny=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	cvCvtColor(src,pImg8u,CV_RGB2GRAY);           //�ҶȻ�
	cvSmooth(pImg8u,pImg8uSmooth,CV_GAUSSIAN,3,0,0);//��˹�˲�
	cvCanny(pImg8uSmooth,pImgCanny,100,200,3);    //��ֵ��
	/*cvDilate(pImgCanny,pImgCanny,0,1);
	cvErode(pImgCanny,pImgCanny,0,1);*/

	row_start=0;
	row_end=0;
	col_start=0;
	col_end=0;

	int row[120];
	int col[340];
	int k;
	k=0;
	bool flag=false;		

	for(j=0;j<pImgCanny->height;j++)           //�ҵ����п�ʼ
	{
		count=0;
		for(i=0;i<pImgCanny->width-1;i++)
		{
			if(S(pImgCanny,i,j)!=S(pImgCanny,i+1,j))  //ͳ��������
				count++;
			if(count>T)
			{
				row[k]=j;
				k++;
				break;
			}
		}
	}

	for(i=0;i<k;i++)          //���ϱ߿�ʼ��3������ʱ��Ϊ����ʼ��
	{
		if((row[i]==row[i+1]-1)&&(row[i+1]==row[i+2]-1)){
			row_start=row[i];
			break;
		}
	}
	cout<<"the start row:"<<row_start<<endl;
	cvLine(pImg8u,cvPoint(0,row_start),cvPoint(src->width,row_start),cvScalar(255,0,0),1,8,0);
	
	
	for(i=k-1;i>row_start;i--)     //���±߿�ʼ��3������ʱ��Ϊ����ʼ��
	{
		if((row[i]==row[i-1]+1)&&(row[i-1]==row[i-2]+1)){
			row_end=row[i];
			break;
		}
	}
	cout<<"the end row:"<<row_end<<endl;
	cvLine(pImg8u,cvPoint(0,row_end),cvPoint(src->width,row_end),cvScalar(255,0,0),1,8,0);



	flag=false;
	for(i=10;i<pImgCanny->width;i++)           //�ҵ����п�ʼ
	{
		count=0;
		for(j=row_start;j<row_end;j++)
		{
			if(S(pImgCanny,i,j)==255)
				count++;
			if(count>T1)
			{
				col_start=i;
				flag=true;
				break;
			}
		}
		if(flag) break;
	}
	cout<<"the start col:"<<col_start<<endl;
	cvLine(pImg8u,cvPoint(col_start,row_start),cvPoint(col_start,row_end),cvScalar(255,0,0),1,8,0);

	flag=false;
	for(i=pImgCanny->width-10;i>col_start;i--)           //�ҵ����п�ʼ
	{
		count=0;
		for(j=row_start;j<row_end;j++)
		{
			if(S(pImgCanny,i,j)==255)
				count++;
			if(count>T1)
			{
				col_end=i;
				flag=true;
				break;
			}
		}
		if(flag) break;
	}
	cout<<"the end col:"<<col_end<<endl;
	cvLine(pImg8u,cvPoint(col_end,row_start),cvPoint(col_end,row_end),cvScalar(255,0,0),1,8,0);

	CvRect ROI_rect;                 //���ͼƬ����Ȥ����
	ROI_rect.x=col_start;
	ROI_rect.y=row_start;
	ROI_rect.width=col_end-col_start;
	ROI_rect.height=row_end-row_start;
	IplImage *pImg8uROI=NULL;         //����Ȥ��ͼƬ
	cvSetImageROI(pImg8u,ROI_rect);
	pImg8uROI=cvCreateImage(cvSize(ROI_rect.width,ROI_rect.height),IPL_DEPTH_8U,1);
	cvCopy(pImg8u,pImg8uROI);
	cvResetImageROI(pImg8u);
	
	int nWidth=409;//(409,90)�ֱ�Ϊ����Ȥͼ��Ŀ����߶�
	int nHeight=90;
	IplImage *pImgResize=NULL;        //��һ���ĻҶ�ͼ
	pImgResize=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
	cvResize(pImg8uROI,pImgResize,CV_INTER_LINEAR); //���Բ�ֵ
	
	int nCharWidth=45;
	int nSpace=12;
	for(i=0;i<7;i++)           //�õ�ÿ���ַ���˫�߽�
	{
		switch(i){
			case 0:
			case 1:
				col[i*2]=i*nCharWidth+i*nSpace;
				col[i*2+1]=(i+1)*nCharWidth+i*nSpace;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				col[i*2]=i*nCharWidth+i*nSpace+22;
				col[i*2+1]=(i+1)*nCharWidth+i*nSpace+22;
				break;
		}

	}
	for(i=0;i<14;i++)        //����ÿ���ַ�������
	{
		cvLine(pImgResize,cvPoint(col[i],0),cvPoint(col[i],nHeight),cvScalar(255,0,0),1,8,0);
		//cout<<col[i*2]<<" "<<col[2*i+1]<<" ";
	}

	IplImage *pImgCharOne=NULL;
	IplImage *pImgCharTwo=NULL;
	IplImage *pImgCharThree=NULL;
	IplImage *pImgCharFour=NULL;
	IplImage *pImgCharFive=NULL;
	IplImage *pImgCharSix=NULL;
	IplImage *pImgCharSeven=NULL;
	pImgCharOne=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);
	pImgCharTwo=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);
	pImgCharThree=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);
	pImgCharFour=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);
	pImgCharFive=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);
	pImgCharSix=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);
	pImgCharSeven=cvCreateImage(cvSize(nCharWidth,nHeight),IPL_DEPTH_8U,1);

	CvRect ROI_rect1;
	ROI_rect1.x=col[0];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharOne,NULL); //��ȡ��1���ַ�
	cvResetImageROI(pImgResize);

	ROI_rect1.x=col[2];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharTwo,NULL); //��ȡ��2���ַ�
	cvResetImageROI(pImgResize);

	ROI_rect1.x=col[4];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharThree,NULL); //��ȡ��3���ַ�
	cvResetImageROI(pImgResize);

	ROI_rect1.x=col[6];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharFour,NULL); //��ȡ��4���ַ�
	cvResetImageROI(pImgResize);

	ROI_rect1.x=col[8];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharFive,NULL); //��ȡ��5���ַ�
	cvResetImageROI(pImgResize);

	ROI_rect1.x=col[10];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharSix,NULL); //��ȡ��6���ַ�
	cvResetImageROI(pImgResize);

	ROI_rect1.x=col[12];
	ROI_rect1.y=0;
	ROI_rect1.width=nCharWidth;
	ROI_rect1.height=nHeight;
	cvSetImageROI(pImgResize,ROI_rect1);
	cvCopy(pImgResize,pImgCharSeven,NULL); //��ȡ��7���ַ�
	cvResetImageROI(pImgResize);


	/*cvLine(src,cvPoint(col_start,row_start),cvPoint(col_end,row_start),cvScalar(255,0,0),3,8,0); //��ԭͼ�еõ���������
	cvLine(src,cvPoint(col_start,row_end),cvPoint(col_end,row_end),cvScalar(255,0,0),3,8,0);
	cvLine(src,cvPoint(col_start,row_start),cvPoint(col_start,row_end),cvScalar(255,0,0),3,8,0);
	cvLine(src,cvPoint(col_end,row_start),cvPoint(col_end,row_end),cvScalar(255,0,0),3,8,0);*/
	

	pImgHist=cvCreateImage(cvSize(300,200),8,1);   //��ֱ��ͼ
	CvHistogram *hist=cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
	cvCalcHist(&pImg8uSmooth,hist,0,0);
	cvGetMinMaxHistValue(hist,&min_value,&max_value,&min_dx,&max_dx);

	cout<<min_value<<"  "<<max_value<<"  "<<min_dx<<"  "<<max_dx<<endl;
	cvScale(hist->bins,hist->bins,((double)pImgHist->height)/max_value,0);
	cvSet(pImgHist,cvScalarAll(255),0);
	bin_w=cvRound((pImgHist->width)/hist_size);
	for(i=0;i<hist_size;i++)
	{
		cvRectangle(pImgHist,cvPoint(i*bin_w,pImgHist->height),
			cvPoint((i+1)*bin_w,pImgHist->height-cvRound(cvGetReal1D(hist->bins,i))),
			cvScalarAll(0),-1,8,0);
	}
	

	CvMemStorage *storage=NULL;
	CvSeq *lines=NULL;
	storage=cvCreateMemStorage(0);
	lines=cvHoughLines2(pImgCanny,storage,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,80,30,20);
	/*for(i=0;i<lines->total;i++)
	{
		CvPoint *line=(CvPoint*)cvGetSeqElem(lines,i);
		cvLine(src,line[0],line[1],CV_RGB(255,0,0),3,8,0);
	}*/

	/*cvNamedWindow("src",1);
	cvShowImage("src",src);
	cvNamedWindow("pimg8u",1);
	cvShowImage("pimg8u",pImg8u);
	cvNamedWindow("canny",1);
	cvShowImage("canny",pImgCanny);*/
	cvNamedWindow("resize",1);
	cvShowImage("resize",pImgResize);
	cvNamedWindow("roi",1);
	cvShowImage("roi",pImg8uROI);
	/*cvNamedWindow("hist",1);
	cvShowImage("hist",pImgHist); */
	cvNamedWindow("one",CV_WINDOW_AUTOSIZE);
	cvShowImage("one",pImgCharOne);
	cvNamedWindow("two",1);
	cvShowImage("two",pImgCharTwo);
	cvNamedWindow("three",1);
	cvShowImage("three",pImgCharThree);
	cvNamedWindow("four",1);
	cvShowImage("four",pImgCharFour);
	cvNamedWindow("five",1);
	cvShowImage("five",pImgCharFive);
	cvNamedWindow("six",1);
	cvShowImage("six",pImgCharSix);
	cvNamedWindow("seven",1);
	cvShowImage("seven",pImgCharSeven);
	
	cvWaitKey(0);
	/*cvReleaseImage(&src);
	cvReleaseImage(&pImg8u);
	cvReleaseImage(&pImg8uSmooth);
	cvReleaseImage(&pImgCanny);
	cvReleaseImage(&pImgHist);*/
	cvReleaseImage(&pImgResize);
	cvReleaseImage(&pImg8uROI);
	cvReleaseImage(&pImgCharOne);
	cvReleaseImage(&pImgCharTwo);
	cvReleaseImage(&pImgCharThree);
	cvReleaseImage(&pImgCharFour);
	cvReleaseImage(&pImgCharFive);
	cvReleaseImage(&pImgCharSix);
	cvReleaseImage(&pImgCharSeven);
	cvDestroyWindow("roi");
	/*cvDestroyWindow("src");
	cvDestroyWindow("pimg8u");
	cvDestroyWindow("canny");
	cvDestroyWindow("hist");*/
	cvDestroyWindow("resize");
	cvDestroyWindow("one");
	cvDestroyWindow("two");
	cvDestroyWindow("three");
	cvDestroyWindow("four");
	cvDestroyWindow("five");
	cvDestroyWindow("six");
	cvDestroyWindow("seven");
}
