#include "include/plate.h"

int main(int argc, char *argv [])
{
	
	/*********************************************准备工作*****************************************/
	IplImage * img_car = NULL;
	IplImage * img_car_after_resize = NULL;
	IplImage * img_after_preprocess;
	IplImage * img_plate = NULL;
	IplImage * img_after_resize;
	IplImage * last_character;

	List rects; /*保存预选车牌位置的列表*/
	double scale = -1; /*在尺寸归一化时要用到*/
	int width = 0, height = 0; /*最开始时候的尺寸归一化的长宽*/
	int number = -1;	/*最后一个字符的数字结果*/

#if 1
	cvNamedWindow("img_car", 1);
//	cvNamedWindow("img_car_after_resize", 1);
	//cvNamedWindow("img_after_preprocess", 1);
	//cvNamedWindow("img_plate", 1);
#endif

	if ((img_car = cvLoadImage(argv[1], -1)) == NULL) {
		fprintf(stderr, "Can not open car image file!\n");
		exit(-1);
	}

	/*****************************************开始进行图像处理***************************************/
	/*为了便于对图像进行统一处理,先对图像尺寸进行处理,让图像的尺寸大小合适,
	  一般大概大小为640*480规格的,所以只需要大概按照这个比例进行resize
	 */

	/*用cvResize函数进行处理即可*/
	scale = 1.0 * 640 / img_car->width;
	width = scale * img_car->width;
	height = scale * img_car->height;
	img_car_after_resize = cvCreateImage(cvSize(width, height), img_car->depth, img_car->nChannels);
	cvResize(img_car, img_car_after_resize);			/*对尺寸进行归一化*/

	/*图像预处理:输入为尺寸归一化后的车牌图像,输出为一张img_after_preprocess.bmp图像*/
	preprocess_car_img(img_car_after_resize);

	/*读取img_after_preprocess.bmp图像*/
	img_after_preprocess = cvLoadImage("img_after_preprocess.bmp", -1);

#if 1
	/*显示预处理完成后的图像*/
	cvShowImage("img_car", img_car_after_resize);
	//cvShowImage("img_after_preprocess", img_after_preprocess);
#endif
	
	rects = get_location(img_car_after_resize);			/*得到车牌的位置,起初设计阶段是可以有多个预选位置,但是后来发现不用,所以rects其实只有一个位置*/
	/*由于在get_location中返回的是头结点的next节点,所以这里的参数不用rects->next*/
	assert(rects != NULL);
	get_plate_image(img_car_after_resize, rects);		/*得到车牌的图像*/

	img_plate = cvLoadImage("plate_img0.bmp", -1);		/*上面那个函数中得到的plate_img.bmp图像*/
	if (img_plate == NULL) {
		fprintf(stderr, "Can not open plate image file!\n");
		exit(-1);
	}

	resize_image(img_plate,img_after_resize, 5);

	img_after_resize = cvLoadImage("plate_img_after_resize.bmp", -1);
	preprocess_plate_image(img_after_resize);
		printf("3\n\n\n");
	get_character(img_after_resize);

	printf("\n\n\n\n\nhere\n\n\n");

	last_character = cvLoadImage("last_character.bmp", -1);
	if (last_character == NULL) {
		fprintf(stderr, "Can not open last character image!\n");
		exit(-1);
	}
	number = character_recognizing(last_character);

	cvWaitKey(0);
	return 0;
}













