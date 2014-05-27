#include "cv.h"
#include <math.h>
#include "highgui.h"

void sobel(IplImage * src_img, IplImage * dst_img, int flag);
void show_image_value_one(IplImage * img);
void create_image(IplImage * img);
void Mysobel(IplImage * src_img, IplImage * dst_img, int flag);
void Sobel_ind_fast( IplImage* img, IplImage* dst);

int dx[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
int dy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

int main(int argc, char * argv[])
{
	IplImage * test_img;
	IplImage * sobel_img;
	IplImage * cvsobel_img;
	cvNamedWindow("test_img");
	cvNamedWindow("sobel_img");
	cvNamedWindow("cvsobel_img");


	test_img = cvLoadImage(argv[1]);
//	test_img = cvCreateImage(cvSize(5, 5), IPL_DEPTH_16S, 1);
	sobel_img = cvCreateImage(cvGetSize(test_img), IPL_DEPTH_8U, 1);
	cvsobel_img = cvCreateImage(cvGetSize(test_img), IPL_DEPTH_8U, 1);


//	create_image(test_img);

//	show_image_value_one(test_img);

//	sobel(test_img, sobel_img, 1);


	cvCvtColor(test_img, cvsobel_img, CV_BGR2GRAY);
//	Mysobel(cvsobel_img, sobel_img, 1);
	Sobel_ind_fast(cvsobel_img, sobel_img);

//	Mysobel(test_img, sobel_img, 0);

	show_image_value_one(sobel_img);

	cvSobel(cvsobel_img, cvsobel_img, 1, 1, 3);

	puts("\n\n\nhehn\n\n");
	//show_image_value_one(cvsobel_img);
	cvShowImage("test_img", test_img);
	cvShowImage("sobel_img", sobel_img);
	cvShowImage("cvsobel_img", cvsobel_img);
	cvWaitKey(0);

	return 0;
}


void sobel(IplImage * src_img, IplImage * dst_img, int flag)
{
	int a00, a01, a02;
	int a10, a11, a12;
	int a20, a21, a22;
	int gx, gy;

	for (int i = 1; i < src_img->height - 2; i++){
		unsigned char * srow0 = (unsigned char *)(src_img->imageData + src_img->widthStep * (i - 1));

		unsigned char * srow1 = (unsigned char *)(src_img->imageData + src_img->widthStep * (i - 0));

		unsigned char * srow2 = (unsigned char *)(src_img->imageData + src_img->widthStep * (i + 1));
		unsigned  char * drow = (unsigned char *)(dst_img->imageData + dst_img->widthStep * i);

		for(int j = 1; j < src_img->width - 2; j++) {
			a00 = srow0[j - 1];
			a01 = srow0[j - 0];
			a02 = srow0[j + 1];
			
			a10 = srow1[j - 1];
			a11 = srow1[j - 0];
			a12 = srow1[j + 1];

			a20 = srow2[j - 1];
			a21 = srow2[j - 0];
			a22 = srow2[j + 1];

//			printf("(%d %d %d)\n(%d %d %d)\n(%d %d %d)\n", a00, a01, a02, a10, a11, a12, a20,a21, a22);

			gx = a02 * 1 + a12 * 2 + a22 * 1 - (a00 * 1 + a10 * 2 + a20 * 1);
			gy = a20 * 1 + a21 * 2 + a22 * 1 - (a00 * 1 + a01 * 2 + a02 * 1);
			if (flag = 1)
			  drow[j] = gx;
			else if (flag = 0)
			  drow[j] = gy;

//			printf("gx: %d gy: %d\n drow:%d\n", gx, gy, drow[j]);

		}
		puts("\n");
	}
}
void show_image_value_one(IplImage * img)
{
	int i, j;
	FILE * fp_value;

	fp_value = fopen("value.txt", "a");

	for (i = 0; i < img->height; i ++) {
		unsigned  char * prow = (unsigned  char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width; j++) {
			fprintf(fp_value, "%d ", prow[j]);
		}
		fprintf(fp_value, "\n");

	}

	fprintf(fp_value, "\n");
	fclose(fp_value);
}

void create_image(IplImage * img)
{
	FILE * fp_test = fopen("test.txt", "r");

	int i, j;
	int count;
	fscanf(fp_test, "%d", &count);;

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
		for (j = 0; j < img->width; j++){
			prow[j] = count;	
			fscanf(fp_test, "%d", &count);;
		}

	}
}
 void Mysobel(IplImage* gray, IplImage* gradient, int flag)
 {
     /* Sobel template
     a00 a01 a02
     a10 a11 a12
     a20 a21 a22
     */
 
     unsigned char a00, a01, a02, a20, a21, a22;
    unsigned char a10, a11, a12;
 
     for (int i=1; i<gray->height-1; ++i)
     {
         for (int j=1; j<gray->width-1; ++j)
         {
             CvScalar color = cvGet2D(gray, i, j);
     
             a00 = cvGet2D(gray, i-1, j-1).val[0];
             a01 = cvGet2D(gray, i-1, j).val[0];
             a02 = cvGet2D(gray, i-1, j+1).val[0];
 
             a10 = cvGet2D(gray, i, j-1).val[0];
             a11 = cvGet2D(gray, i, j).val[0];
             a12 = cvGet2D(gray, i, j+1).val[0];
 
             a20 = cvGet2D(gray, i+1, j-1).val[0];
             a21 = cvGet2D(gray, i+1, j).val[0];
             a22 = cvGet2D(gray, i+1, j+1).val[0];
             

//			  printf("(%d %d %d)\n(%d %d %d)\n(%d %d %d)\n", a00, a01, a02,a10, a11, a12, a20, a21, a22);
	
             // x方向上的近似导数
             double ux = a20 * (1) + a21 * (2) + a22 * (1) 
                 + (a00 * (-1) + a01 * (-2) + a02 * (-1));
 
             // y方向上的近似导数
             double uy = a02 * (1) + a12 * (2) + a22 * (1)
                 + a00 * (-1) + a10 * (-2) + a20 * (-1);
#if 0 
			 if (flag == 0)
				color.val[0] = uy;
			 else
			   color.val[0] = ux;
#endif
			 color.val[0] = sqrt(ux * ux + uy * uy);

//			 color.val[0] = sqrt(ux * ux + uy * uy);
             cvSet2D(gradient, i, j, color);
         }
     }
 }

void Sobel_ind_fast( IplImage* img, IplImage* dst)
{
		CvScalar s;
			for (int i=1; i < img->height-2; i++)
					for (int j=1; j < img->width-2; j++)
							{
											// apply kernel in X and Y directions
											int sum_x=0;
														int sum_y=0;
																	for(int m=-1; m<=1; m++)
																					for(int n=-1; n<=1; n++)
																									{
																															s=cvGet2D(img,i+m,j+n); // get the (i,j) pixel value
																																				sum_x+=(int)s.val[0]*dx[m+1][n+1];
																																									sum_y+=(int)s.val[0]*dy[m+1][n+1];
																																													}
																				int sum=abs(sum_x)+abs(sum_y);
																							s.val[0]=(sum>255)?255:sum;
																										cvSet2D(dst,i,j,s); // set the (i,j) pixel value
																												}
}
