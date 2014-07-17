#include <iostream>
#include <time.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

using namespace std;
int main(int argc, char** argv)
{
	void WrongUsage();
 	
	double total_time =0;
	double score= 0.8;
 
	
	IplImage * src= cvLoadImage(argv[1]);

	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane ,v_plane};
 
	/** H 分量划分为16个等级，S分量划分为8个等级 */
	int h_bins = 10, s_bins = 6,v_bins = 6;
  	param = cp.GetParameter("-h");
	if(param)
	{
        h_bins=atoi(param);
	}
  	param = cp.GetParameter("-s");
	if(param)
	{
        s_bins=atoi(param);
	}
  	param = cp.GetParameter("-v");
	if(param)
	{
        v_bins=atoi(param);
	}
	int hist_size[] = {h_bins, s_bins,v_bins};
 
	/** H 分量的变化范围 */
	float h_ranges[] = { 0, 180 }; 
 
	/** S 分量的变化范围*/
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges,v_ranges };
 
	/** 输入图像转换到HSV颜色空间 */
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	/** 创建直方图，二维, 每个维度上均分 */
	CvHistogram * hist = cvCreateHist(3, hist_size, CV_HIST_ARRAY, ranges, 1 );
	/** 根据H,S两个平面数据统计直方图 */
	cvCalcHist( planes, hist, 0, 0 );
 
	/** 获取直方图统计的最大值，用于动态显示直方图 */
	float max_value;
	int maxid[4]={0};
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, maxid );
 
 
	/** 设置直方图显示图像 */
	int height = 520;
	int ivh=height/v_bins;
	int width = (h_bins*s_bins*6);//每个bin6像素
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );
    CvFont font = cvFont(1,1); 

	/** 用来进行HSV到RGB颜色转换的临时单位图像 */
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width / (h_bins * s_bins);
	char dspStr1[80];
	sprintf(dspStr1,"max=%f",max_value);
	cvPutText(hist_img,dspStr1,cvPoint(20,20),&font,cvScalar(0,0xff)); 
	cout<< dspStr1 <<endl;
    float tatal_val=0;
	bool bh=false;
   float htotal=0.0;
   for(int v=0;v<v_bins;v++)
   {
	 for(int h = 0; h < h_bins; h++)
	{
		 bh=false;
		 float hto=0.0;
		 int i=0;
		for(int s = 0; s < s_bins; s++)
		{
			 i = h*s_bins + s;
			/** 获得直方图中的统计次数，计算显示在图像中的高度 */
			float bin_val = cvQueryHistValue_3D( hist, h, s,v );
			int intensity = cvRound(bin_val*ivh/max_value);
            tatal_val+=bin_val;
			hto+=bin_val;
			if(bin_val==max_value)
			{
 	             bh=true; 
               sprintf(dspStr1,"h=%d,s=%d,v=%d",h,s,v);
	           cvPutText(hist_img,dspStr1,cvPoint(20,50),&font,cvScalar(0xff,0xaf)); 
			   cout<< dspStr1 <<endl;
			}
			/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
			float vhr=(v+0.5)*255.f/v_bins;//避免显示黑色 
			cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,(s+0.5)*255.f/s_bins,vhr,0));
			cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color,0,0);
 
			cvRectangle( hist_img, cvPoint(i*bin_w,(v+1)*ivh),
				cvPoint((i+1)*bin_w,(v+1)*ivh - intensity),
				color, -1, 8, 0 );
		}
		if(bh)
			{
 	          htotal=hto;
			}
       cvLine(hist_img,cvPoint((i+1)*bin_w,v*ivh),cvPoint((i+1)*bin_w,(v+1)*ivh),cvScalar(0xA,0xff,0x98));
	}
     cvLine(hist_img,cvPoint(0,(v+1)*ivh),cvPoint(width,(v+1)*ivh),cvScalar(0xA,0xff,0x98));
   }
 	sprintf(dspStr1,"pixels=%f",tatal_val);
	cvPutText(hist_img,dspStr1,cvPoint(20,80),&font,cvScalar(0,0xff)); 
	cout<< dspStr1 <<endl;
	sprintf(dspStr1,"rate=%f",htotal/tatal_val);
	cvPutText(hist_img,dspStr1,cvPoint(20,110),&font,cvScalar(0,0xff,0xff)); 
	cout<< dspStr1 <<endl;
	sprintf(dspStr1,"maxrt=%f",max_value/tatal_val);
	cvPutText(hist_img,dspStr1,cvPoint(20,140),&font,cvScalar(0xff,0xaf));
	cout<< dspStr1 <<endl;
	cvNamedWindow(filename, 1 );
	cvShowImage( filename, src );
 
	cvNamedWindow( "H-S Histogram", 1 );
	cvShowImage( "H-S Histogram", hist_img );
 
	cvWaitKey(0);


 
}
