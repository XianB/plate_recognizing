#include "include/plate.h"

int main(int argc, char *argv [])
{
	
	/*********************************************准备工作*****************************************/
	IplImage * img_car = NULL;
	IplImage * img_after_preprocess;
	IplImage * img_plate = NULL;

	cvNamedWindow("img_car", 1);
	cvNamedWindow("img_after_preprocess", 1);
	cvNamedWindow("img_plate", 1);

	if ((img_car = cvLoadImage(argv[1], -1)) == NULL) {
		fprintf(stderr, "Can not open car image file!\n");
		exit(-1);
	}

	/*****************************************开始进行图像处理***************************************/
	/*先对图像尺寸进行归一化*/
	/*用cvResize函数进行处理即可*/

	/*图像预处理:输入为原始车牌图像,输出为一张img_after_preprocess.bmp图像*/
	preprocess_car_img(img_car);

	/*读取img_after_preprocess.bmp图像*/
	img_after_preprocess = cvLoadImage("img_after_preprocess.bmp", -1);

	/*显示预处理完成后的图像*/
	cvShowImage("img_after_preprocess", img_after_preprocess);
	
	get_location(img_car);


	cvWaitKey(0);
	return 0;
}
