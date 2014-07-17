    #include "highgui.h"  
    #include "cv.h"  
    #include <stdio.h>     
    #include <math.h>    
    #include <string>  
    #include<iostream>  
    using namespace std;  
      
    CvPoint Point;  
    IplImage* img=0;  
    // skin region location using rgb limitation  
    void SkinRGB(IplImage* rgb,IplImage* _dst)  
    {  
        assert(rgb->nChannels==3&& _dst->nChannels==3);  
      
        static const int R=2;  
        static const int G=1;  
        static const int B=0;  
      
        IplImage* dst=cvCreateImage(cvGetSize(_dst),8,3);  
        cvZero(dst);  
      
        for (int h=0;h<rgb->height;h++) {  
            unsigned char* prgb=(unsigned char*)rgb->imageData+h*rgb->widthStep;  
            unsigned char* pdst=(unsigned char*)dst->imageData+h*dst->widthStep;  
            for (int w=0;w<rgb->width;w++) {  
                if ((prgb[R]>95 && prgb[G]>40 && prgb[B]>20 &&  
                    prgb[R]-prgb[B]>15 && prgb[R]-prgb[G]>15/*&& 
                    !(prgb[R]>170&&prgb[G]>170&&prgb[B]>170)*/)||//uniform illumination   
                    (prgb[R]>200 && prgb[G]>210 && prgb[B]>170 &&  
                    abs(prgb[R]-prgb[B])<=15 && prgb[R]>prgb[B]&& prgb[G]>prgb[B])//lateral illumination  
                    ) {  
                        memcpy(pdst,prgb,3);  
                }             
                prgb+=3;  
                pdst+=3;  
            }  
        }  
        cvCopyImage(dst,_dst);  
        cvReleaseImage(&dst);  
    }  
    // skin detection in rg space  
    void cvSkinRG(IplImage* rgb,IplImage* gray)  
    {  
        assert(rgb->nChannels==3&&gray->nChannels==1);  
          
        const int R=2;  
        const int G=1;  
        const int B=0;  
      
        double Aup=-1.8423;  
        double Bup=1.5294;  
        double Cup=0.0422;  
        double Adown=-0.7279;  
        double Bdown=0.6066;  
        double Cdown=0.1766;  
        for (int h=0;h<rgb->height;h++) {  
            unsigned char* pGray=(unsigned char*)gray->imageData+h*gray->widthStep;  
            unsigned char* pRGB=(unsigned char* )rgb->imageData+h*rgb->widthStep;  
            for (int w=0;w<rgb->width;w++)   
            {  
                int s=pRGB[R]+pRGB[G]+pRGB[B];  
                double r=(double)pRGB[R]/s;  
                double g=(double)pRGB[G]/s;  
                double Gup=Aup*r*r+Bup*r+Cup;  
                double Gdown=Adown*r*r+Bdown*r+Cdown;  
                double Wr=(r-0.33)*(r-0.33)+(g-0.33)*(g-0.33);  
                if (g<Gup && g>Gdown && Wr>0.004)  
                {  
                    *pGray=255;  
                }  
                else  
                {   
                    *pGray=0;  
                }  
                pGray++;  
                pRGB+=3;  
            }  
        }  
      
    }  
    // implementation of otsu algorithm  
    // author: onezeros#yahoo.cn  
    // reference: Rafael C. Gonzalez. Digital Image Processing Using MATLAB  
    void cvThresholdOtsu(IplImage* src, IplImage* dst)  
    {  
        int height=src->height;  
        int width=src->width;  
      
        //histogram  
        float histogram[256]={0};  
        for(int i=0;i<height;i++) {  
            unsigned char* p=(unsigned char*)src->imageData+src->widthStep*i;  
            for(int j=0;j<width;j++) {  
                histogram[*p++]++;  
            }  
        }  
        //normalize histogram  
        int size=height*width;  
        for(int i=0;i<256;i++) {  
            histogram[i]=histogram[i]/size;  
        }  
      
        //average pixel value  
        float avgValue=0;  
        for(int i=0;i<256;i++) {  
            avgValue+=i*histogram[i];  
        }  
      
        int threshold;    
        float maxVariance=0;  
        float w=0,u=0;  
        for(int i=0;i<256;i++) {  
            w+=histogram[i];  
            u+=i*histogram[i];  
      
            float t=avgValue*w-u;  
            float variance=t*t/(w*(1-w));  
            if(variance>maxVariance) {  
                maxVariance=variance;  
                threshold=i;  
            }  
        }  
      
        cvThreshold(src,dst,threshold,255,CV_THRESH_BINARY);  
    }  
      
    void cvSkinOtsu(IplImage* src, IplImage* dst)  
    {  
        assert(dst->nChannels==1&& src->nChannels==3);  
      
        IplImage* ycrcb=cvCreateImage(cvGetSize(src),8,3);  
        IplImage* cr=cvCreateImage(cvGetSize(src),8,1);  
        cvCvtColor(src,ycrcb,CV_BGR2YCrCb);  
        cvSplit(ycrcb,0,cr,0,0);  
      
        cvThresholdOtsu(cr,cr);  
        cvCopyImage(cr,dst);  
        cvReleaseImage(&cr);  
        cvReleaseImage(&ycrcb);  
    }  
      
    void cvSkinYUV(IplImage* src,IplImage* dst)  
    {  
        IplImage* ycrcb=cvCreateImage(cvGetSize(src),8,3);  
        //IplImage* cr=cvCreateImage(cvGetSize(src),8,1);  
        //IplImage* cb=cvCreateImage(cvGetSize(src),8,1);  
        cvCvtColor(src,ycrcb,CV_BGR2YCrCb);  
        //cvSplit(ycrcb,0,cr,cb,0);  
      
        static const int Cb=2;  
        static const int Cr=1;  
        static const int Y=0;  
      
        //IplImage* dst=cvCreateImage(cvGetSize(_dst),8,3);  
        cvZero(dst);  
      
        for (int h=0;h<src->height;h++) {  
            unsigned char* pycrcb=(unsigned char*)ycrcb->imageData+h*ycrcb->widthStep;  
            unsigned char* psrc=(unsigned char*)src->imageData+h*src->widthStep;  
            unsigned char* pdst=(unsigned char*)dst->imageData+h*dst->widthStep;  
            for (int w=0;w<src->width;w++) {  
                if ((pycrcb[Cr]<=126||pycrcb[Cr]>=130)&&(pycrcb[Cb]<=126||pycrcb[Cb]>=130))  
                {  
                        memcpy(pdst,psrc,3);  
                }  
                pycrcb+=3;  
                psrc+=3;  
                pdst+=3;  
            }  
        }  
        //cvCopyImage(dst,_dst);  
        //cvReleaseImage(&dst);  
    }  
      
    void cvSkinHSV(IplImage* src,IplImage* dst)  
    {  
        IplImage* hsv=cvCreateImage(cvGetSize(src),8,3);  
        //IplImage* cr=cvCreateImage(cvGetSize(src),8,1);  
        //IplImage* cb=cvCreateImage(cvGetSize(src),8,1);  
        cvCvtColor(src,hsv,CV_BGR2HSV);  
        //cvSplit(ycrcb,0,cr,cb,0);  
      
        static const int V=2;  
        static const int S=1;  
        static const int H=0;  
      
        //IplImage* dst=cvCreateImage(cvGetSize(_dst),8,3);  
        cvZero(dst);  
      
        for (int h=0;h<src->height;h++) {  
            unsigned char* phsv=(unsigned char*)hsv->imageData+h*hsv->widthStep;  
            unsigned char* psrc=(unsigned char*)src->imageData+h*src->widthStep;  
            unsigned char* pdst=(unsigned char*)dst->imageData+h*dst->widthStep;  
            for (int w=0;w<src->width;w++) {  
                if (phsv[H]>=90&&phsv[H]<=135)  
                {  
                        memcpy(pdst,psrc,3);  
                }  
                phsv+=3;  
                psrc+=3;  
                pdst+=3;  
            }  
        }  
        //cvCopyImage(dst,_dst);  
        //cvReleaseImage(&dst);  
    }  
      
    void on_mouse(int event,int x,int y,int flags,void* param )    
    {    
         switch(event)    
         {    
         case CV_EVENT_LBUTTONUP:    
             {    
                 Point=cvPoint(x,y);    
             }    
                cvCircle(img,Point,1,CV_RGB(255,0,0),1);  
                CvScalar HSV=cvGet2D(img,x,y);  
                cout<<"H:"<<HSV.val[0]<<"\t S:"<<HSV.val[1]<<"\t V:"<<HSV.val[2]<<endl;   
             break;    
         }    
        
    //printf("( %d, %d) ",x,y);    
    //printf("The Event is : %d ",event);    
    //printf("The flags is : %d ",flags);    
    //printf("The param is : %d\n",param);    
    }  
    int main(int argc, char * argv[])  
    {     
          
        IplImage* img0= cvLoadImage(argv[1]); //随便放一张jpg图片在D盘或另行设置目录  
        img=cvCreateImage(cvSize(400,300),8,3);  
        cvResize(img0,img);  
        IplImage* dstRGB=cvCreateImage(cvGetSize(img),8,3);  
        IplImage* dstRG=cvCreateImage(cvGetSize(img),8,1);  
        IplImage* dst_crotsu=cvCreateImage(cvGetSize(img),8,1);  
        IplImage* dst_YUV=cvCreateImage(cvGetSize(img),8,3);  
        IplImage* dst_HSV=cvCreateImage(cvGetSize(img),8,3);  
      
      
        cvNamedWindow("inputimage", CV_WINDOW_AUTOSIZE);  
        cvShowImage("inputimage", img);  
        cvWaitKey(0);  
        /* 
        SkinRGB(img,dstRGB); 
        cvNamedWindow("outputimage1", CV_WINDOW_AUTOSIZE); 
        cvShowImage("outputimage1", dstRGB); 
        cvWaitKey(0); 
        cvSkinRG(img,dstRG); 
        cvNamedWindow("outputimage2", CV_WINDOW_AUTOSIZE); 
        cvShowImage("outputimage2", dstRG); 
        cvWaitKey(0); 
        cvSkinOtsu(img,dst_crotsu); 
        cvNamedWindow("outputimage3", CV_WINDOW_AUTOSIZE); 
        cvShowImage("outputimage3", dst_crotsu); 
        cvWaitKey(0); 
         
        cvSkinYUV(img,dst_YUV); 
        cvNamedWindow("outputimage4", CV_WINDOW_AUTOSIZE); 
        cvShowImage("outputimage4", dst_YUV); 
        //cvSaveImage("D:/skin04.jpg",dst_YUV); 
        cvWaitKey(0); 
        */  
        cvSkinHSV(img,dst_HSV);  
        cvNamedWindow("outputimage5", CV_WINDOW_AUTOSIZE);  
        cvShowImage("outputimage5", dst_HSV);  
        cvSaveImage("image.bmp",dst_HSV);  
        cvWaitKey(0);  
          
        return 0;  
    }  
