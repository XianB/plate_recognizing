#include "cv.h"
#include "highgui.h"
int ImageStretchByHistogram(IplImage *src,IplImage *dst);

int main(int argc, char* argv[])
{
    IplImage * pImg;   
    pImg=cvLoadImage(argv[1],-1);

//创建一个灰度图像
    IplImage* GrayImage = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
    IplImage* dstGrayImage = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
    cvCvtColor(pImg, GrayImage, CV_BGR2GRAY);
    ImageStretchByHistogram(GrayImage,dstGrayImage);

  cvNamedWindow( "dstGrayImage", 1 ); //创建窗口
        cvNamedWindow( "GrayImage", 1 ); //创建窗口
        cvShowImage( "dstGrayImage", dstGrayImage ); //显示图像
        cvShowImage( "GrayImage", GrayImage ); //显示图像
        cvWaitKey(0); //等待按键

  cvDestroyWindow( "dstGrayImage" );//销毁窗口
        cvDestroyWindow( "GrayImage" );//销毁窗口
        cvReleaseImage( &pImg ); //释放图像
        cvReleaseImage( &GrayImage ); //释放图像
        cvReleaseImage( &dstGrayImage ); //释放图像

  return 0;
}

int ImageStretchByHistogram(IplImage *src,IplImage *dst)
/*************************************************
  Function:       
  Description:     因为摄像头图像质量差，需要根据直方图进行图像增强，
                   将图像灰度的域值拉伸到0-255
  Calls:         
  Called By:     
  Input:           单通道灰度图像                 
  Output:          同样大小的单通道灰度图像
  Return:         
  Others:           http://www.xiaozhou.net/ReadNews.asp?NewsID=771
  DATE:               2007-1-5
*************************************************/
{
    //p[]存放图像各个灰度级的出现概率；
    //p1[]存放各个灰度级之前的概率和，用于直方图变换；
    //num[]存放图象各个灰度级出现的次数;

    assert(src->width==dst->width);
    float p[256],p1[256],num[256];
    //清空三个数组
    memset(p,0,sizeof(p));
    memset(p1,0,sizeof(p1));
    memset(num,0,sizeof(num));

    int height=src->height;
    int width=src->width;
    long wMulh = height * width;

    //求存放图象各个灰度级出现的次数
    // to do use openmp
    for(int x=0;x < width; x++){
        for(int y=0;y < height; y++) {
            uchar v=((uchar*)(src->imageData + src->widthStep*y))[x];
            num[v]++;
        }
    }

    //求存放图像各个灰度级的出现概率
    for(int i=0;i<256;i++)
    {
        p[i]=num[i]/wMulh;
    }

    //求存放各个灰度级之前的概率和
    for(int i=0;i<256;i++)
    {
        for(int k=0;k<=i;k++)
            p1[i]+=p[k];
    }

    //直方图变换
    // to do use openmp
    for(int x=0;x < width; x++)    {
        for(int y=0;y < height; y++ )    {
            uchar v=((uchar*)(src->imageData + src->widthStep*y))[x];
            ((uchar*)(dst->imageData + dst->widthStep*y))[x]= p1[v]*255+0.5;           
        }
    }

    return 0;

} 
