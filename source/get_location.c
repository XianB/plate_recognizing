#include "include/plate.h"
#include "include/List.h"

static void dilate_erode_x(IplImage * img_after_threshold, IplImage * img_final); 
static void dilate_erode_y(IplImage * img_final);
/*
输入:预处理完成后的车辆图像
输出:车牌定位的位置
作者:唐显斌
版本:v1.2
基本思路:
1.找到膨胀腐蚀后的所有外接矩形轮廓
2.根据矩形轮廓的形状比例进行筛选
3.根据颜色以及形状比例进行筛选
4.用循环控制膨胀腐蚀的次数,找到最优矩形即为车牌位置
 */

List get_location(IplImage * img_car)
{
	List rects = create_list();
	List rects_final = create_list();
	IplImage * img_after_preprocess = cvLoadImage("img_after_preprocess.bmp", -1);
	if (img_after_preprocess == NULL) {
		fprintf(stderr, "Can not load image -- img_after_preprocess.bmp!\n");
		exit(-1);
	}

	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contours = NULL;

	/*先进行膨胀腐蚀*/
	dilate_erode_x(img_after_preprocess, img_after_preprocess);
	dilate_erode_y(img_after_preprocess);
	/*找到所有的矩形轮廓*/
	get_contour_rect(img_after_preprocess, rects, storage, contours);
	/*按照形状进行矩形筛选*/
	filter_rect_by_shape(rects->next, rects_final);
	/*注意rects有头结点,所以传进去的时候忽略掉头结点*/
	draw_contour_rect(img_after_preprocess, rects_final->next);

	return rects_final->next;
}

/*自定义膨胀腐蚀操作,在循环中使用可以增加定位准确度*/
void dilate_erode_x(IplImage * img_after_threshold, IplImage * img_final) {
	/*自定义1*3的核进行X方向的膨胀腐蚀*/
	IplConvKernel* kernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);
	cvDilate(img_after_threshold, img_final, kernal, 3);/*X方向膨胀连通数字*/
	cvErode(img_final, img_final, kernal, 1);/*X方向腐蚀去除碎片*/
	cvDilate(img_final, img_final, kernal, 3);/*X方向膨胀回复形态*/
}

void dilate_erode_y(IplImage * img_final) {
	/*自定义3*1的核进行Y方向的膨胀腐蚀*/
	IplConvKernel* kernal = cvCreateStructuringElementEx(1, 3, 0, 1, CV_SHAPE_RECT);
	cvErode(img_final, img_final, kernal, 3);/*Y方向腐蚀去除碎片*/
	cvDilate(img_final, img_final, kernal, 3);/*回复形态*/
}



