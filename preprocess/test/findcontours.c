/*这个程序的二值化做的很好，可以参考一下，用来做车牌的边缘检测，因为他的车牌边缘很清楚！*/

#include "cv.h"
#include "highgui.h"

IplImage* g_image=NULL;
IplImage* g_gray=NULL;
int g_thresh=100;           //初始阈值
CvMemStorage * g_storage=NULL;

void on_trackbar(int)     //回调函数
{
    if(g_storage==NULL)
    {
         g_gray=cvCreateImage(cvGetSize(g_image),8,1);
         g_storage=cvCreateMemStorage(0);
    }
    else
        cvClearMemStorage(g_storage);
   CvSeq* contours=0;
   cvCvtColor(g_image,g_gray,CV_BGR2GRAY);
   cvThreshold(g_gray,g_gray,g_thresh,255,CV_THRESH_BINARY);
   cvFindContours(g_gray,g_storage,&contours);    //找轮廓
   cvZero(g_gray);
   if(contours)
       cvDrawContours(g_gray,contours,cvScalarAll(255),cvScalarAll(255),100);   //画轮廓
   cvShowImage("Contours",g_gray);
 }

int main(int argc, char * argv[])
{
    if(argc !=2 || !(g_image=cvLoadImage(argv[1])))
        return -1;
   cvNamedWindow("Contours",0);
   cvCreateTrackbar("Threshold","Contours",&g_thresh,255,on_trackbar);    //阈值0-255
   on_trackbar(0);
   cvWaitKey();
   return 0;
}
