    #include "cv.h"  
    #include "highgui.h"  
    #include "cxcore.h"  
    #include <stdio.h>  
    #include <math.h>  
    #include <string.h>  
    #include <string>  
    using namespace std;  
      
      
    CvPoint pt[4];  
    IplImage* img = 0;  
    IplImage* img0 = 0;  
    const char* wndname = "Demo";  
      
      
    void FindContours(IplImage* src, IplImage * img);  
      
    int main(int argc, char** argv)  
    {  
      
			double scale = -1;
			int width = -1;
			int height = -1;
            // load i-th image  
            img0 = cvLoadImage( argv[1], 0 );  
            if( !img0 )  
            {  
                printf("Couldn't load %s/n", argv[1] );  
            }  
			scale = 1.0 * 640 / img0->width;
			width = scale * img0->width;
			height = scale * img0->height;
			printf("%lf %d %d\n", scale, width, height);
			img = cvCreateImage(cvSize(width, height), img0->depth, img0->nChannels);
            cvResize(img0,img);  
			printf("\n\nhere\n\n");
            cvNamedWindow("input",1);  
            cvShowImage("input",img);  
            cvSmooth(img,img,CV_MEDIAN);  
            //cvPyrDown( img, pyr, 7 );  
            //cvPyrUp( pyr, img, 7 );  
      
      
            //img = cvCloneImage( img0 );  
            IplImage* imgS=cvCreateImage(cvGetSize(img),IPL_DEPTH_16S,1);  
            IplImage* imgTh=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
            IplImage* temp=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);  
              
      
            cvSobel(img,imgS,2,0,3);  
            cvNormalize(imgS,imgTh,255,0,CV_MINMAX);  
      
            cvNamedWindow(wndname,1);  
      
            cvNamedWindow("Sobel",1);  
            cvShowImage("Sobel",imgTh);  
      
      
            //cvAdaptiveThreshold(imgTh,imgTh,255,0,0,5,5);  
            cvThreshold( imgTh, imgTh, 100, 255, CV_THRESH_BINARY );  
              
#if 1
            for (int k=0; k<img->height; k++)  
      
                for(int j=0; j<img->width; j++)  
      
                {  
      
                    imgTh->imageData[k*img->widthStep+j] = 255 - imgTh->imageData[k*img->widthStep+j];  
      
                }  
			cvNamedWindow("test", 1);
			cvShowImage("test", imgTh);
#endif 
                  
            cvNamedWindow("Th",1);  
            cvShowImage("Th",imgTh);  
#if 1
			//这里是关键!
            IplConvKernel* K=cvCreateStructuringElementEx(3,1,0,0,CV_SHAPE_RECT);  
            IplConvKernel* K1=cvCreateStructuringElementEx(3, 3, 0,0,CV_SHAPE_RECT);  
              
            cvMorphologyEx(imgTh,imgTh,temp,K,CV_MOP_CLOSE,10);  
            cvMorphologyEx(imgTh,imgTh,temp,K1,CV_MOP_OPEN,1);  
            //cvDilate(imgTh,imgTh,K,15);  
            //cvErode(imgTh,imgTh,K,15);  
            cvShowImage(wndname,imgTh);  
            string a=argv[1];  
            a.insert(15,"sobel_normal");  
            //cvSaveImage(a.c_str(),imgTh);  
            //cvWaitKey(0);  
		cvNamedWindow("before_", 1);
		cvShowImage("before_", imgTh);
#endif 
            FindContours(imgTh, img);  
      
            //cvShowImage(wndname,imgTh);  
              
              
      
              
    }  
      
    void FindContours(IplImage* src, IplImage * img)  
    {  
        CvMemStorage* storage = cvCreateMemStorage(0);  
        IplImage* dst = cvCreateImage( cvGetSize(src), 8, 3);  
		IplImage * plate_img = NULL;
        cvCvtColor(src,dst,CV_GRAY2BGR);  
        CvScalar color = CV_RGB( 255, 0, 0);  
        CvSeq* contours=0;  
          
         //建立一个空序列存储每个四边形的四个顶点  
       // CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );  
      
        //cvFindContours( src, storage, &contours, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );//外界边界h_next 和 孔用v_next连接  
        cvFindContours( src, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );  
         for( ; contours != 0; contours = contours->h_next)  
                {  
                    //使用边界框的方式  
                    CvRect aRect = cvBoundingRect( contours, 1 );  
                    int tmparea=aRect.height*aRect.height;  
                    if (((double)aRect.width/(double)aRect.height>3)  
                    && ((double)aRect.width/(double)aRect.height<6)&& tmparea>=200&&tmparea<=3500)  
                {  
                    cvRectangle(dst,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width ,aRect.y+aRect.height),color,2);  

					plate_img = cvCreateImage(cvSize(aRect.width, aRect.height), img->depth, img->nChannels);
					cvSetImageROI(img, aRect);
					cvCopy(img, plate_img);
					cvSaveImage("plate.bmp", plate_img);
					cvResetImageROI(img);
                    //cvDrawContours( dst, contours, color, color, -1, 1, 8 );  
                }  
            }  
         cvNamedWindow("contour",1);  
         cvShowImage("contour",dst);  
		 cvNamedWindow("plate", 1);
		 cvShowImage("plate", plate_img);
         cvWaitKey(0);  
                    //多边形曲线逼近方法  
                    /* 
                    //用指定精度逼近多边形曲线  
                    result = cvApproxPoly( contours, sizeof(CvContour), storage, 
                        CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 ); 
     
                    if( result->total == 4 && 
                        fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&   //cvContourArea计算整个轮廓或部分轮廓的面积  
                        cvCheckContourConvexity(result) )                   //CheckContourConvexity 
                    { 
                    */  
           
    }  
