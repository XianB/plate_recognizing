#include "../include/plate.h"

void histogram_analysing(CvHistogram *hist, int bins);

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
            for(int i=0;i<9;i++)
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


int main(int argc, char * argv[])
{
	IplImage * img_rgb = NULL;
	IplImage * img_hsv = NULL;
	IplImage * img_h = NULL;
	IplImage * img_s = NULL;
	IplImage * img_v = NULL;

	if ((img_rgb = cvLoadImage(argv[1])) == NULL) {
		fprintf(stderr, "Error ! Can not open file %s.\n", argv[1]);
		exit(-1);
	}

	img_hsv = cvCreateImage(cvGetSize(img_rgb), img_rgb->depth, img_rgb->nChannels);
	cvCvtColor(img_rgb, img_hsv, CV_RGB2HSV); 

	int dims = 1;
	int bin_h = 10;

	float range_h[] = {0, 255};

	float * ranges_h[] = {range_h};

	CvHistogram * hist_h = cvCreateHist(dims, &bin_h, CV_HIST_ARRAY, ranges_h, 1);

	cvClearHist(hist_h);

	img_h = cvCreateImage(cvGetSize(img_rgb), 8, 1);
	img_s = cvCreateImage(cvGetSize(img_rgb), 8, 1);
	img_v = cvCreateImage(cvGetSize(img_rgb), 8, 1);

	cvSplit(img_rgb, img_h, img_s, img_v, NULL);

	cvNamedWindow("test_h", 1);
	cvShowImage("test_h", img_h);
	cvWaitKey(0);

	cvCalcHist(&img_h, hist_h, 0, 0);
	DrawHistogram(hist_h);
	histogram_analysing(hist_h, bin_h);


	return 0;
}



void histogram_analysing(CvHistogram *hist, int bins)
{
	int i = 0;
	int sum = 0;
	int *ar = (int *)malloc(sizeof(int) * (bins + 1));
	for (i = 0; i < bins; i++) {
		float histValue = cvQueryHistValue_1D(hist , i);
		ar[i] = (int)histValue;
		sum += ar[i];
		printf("%d\n", ar[i]);
	}

	for (i = 0; i < bins; i++) {
		printf("bin %d: %.2lf%%\n", i, 1.0 * ar[i] / sum);
	}
}









