#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <assert.h>


    //绘制直方图
    IplImage* DrawHistogram(CvHistogram* hist , float scaleX = 1 , float scaleY = 1)
    {

            //获取直方图中极大值
            float histMax = 0;
            cvGetMinMaxHistValue(hist , 0 ,&histMax ,0 ,0);

            //创建图像 该图像用于显示直方图
            IplImage* imgHist = cvCreateImage(cvSize(256*scaleX , 64*scaleY) , 8 ,1);

            //图像置零
            cvZero(imgHist);

            //依次绘制直方图的bin
            for(int i=0;i<255;i++)
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
            int size =256; //bin的个数
            float range[] = {0,255}; //取值范围
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

            //计算B通道 直方图
            cvCalcHist(&img_v , hist , 0 , 0 );

            //绘制B通道直方图  结果保存在hist_v
            IplImage* hist_v = DrawHistogram(hist);

            //将B通道的直方图数据清空
            cvClearHist(hist);


            //计算G通道直方图
            cvCalcHist(&img_s , hist , 0 , 0 );

            //绘制G通道直方图  hist_s
            IplImage* hist_s = DrawHistogram(hist);

            //将G通道的直方图数据清空
            cvClearHist(hist);

            //计算R通道 直方图
            cvCalcHist(&img_h , hist , 0 , 0 );

            //绘制R通道直方图  hist_h
            IplImage* hist_h = DrawHistogram(hist);

            //将R通道的直方图数据清空
            cvClearHist(hist);

            cvNamedWindow("v");
            cvNamedWindow("s");
            cvNamedWindow("h");

           cvShowImage("v"  , hist_v);
            cvShowImage("s"  , hist_s);
            cvShowImage("h"  , hist_h);

			char filename_h[20];
			char filename_s[20];
			char filename_v[20];

			sprintf(filename_h, "%c_h.bmp", argv[1][24]);
			sprintf(filename_s, "%c_s.bmp", argv[1][24]);
			sprintf(filename_v, "%c_v.bmp", argv[1][24]);

			cvSaveImage(filename_h, hist_h);
			cvSaveImage(filename_s, hist_s);
			cvSaveImage(filename_v, hist_v);

            cvWaitKey(0);


            //释放资源

            //(1) 释放源图像
            cvReleaseImage(&src);

            //(2) 释放三个通道的图像
            cvReleaseImage(&img_h);
            cvReleaseImage(&img_s);
            cvReleaseImage(&img_v);

            //(3)释放三个通道直方图
            cvReleaseImage(&hist_v);
            cvReleaseImage(&hist_s);
            cvReleaseImage(&hist_h);

            //(4)释放直方图空间
            cvReleaseHist(&hist);

            return 0;
    }
