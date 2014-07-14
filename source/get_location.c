#include "include/plate.h"
#include "include/List.h"

static void get_contour_rect(IplImage * src_img, List  rects, CvMemStorage * storage, CvSeq * contours);
static void draw_contour_rect(IplImage * src_img, List  rects);
static void dilate_erode_x(IplImage * img_after_threshold, IplImage * img_final); 
static void dilate_erode_y(IplImage * img_final); 
static void filter_rect_by_shape(List src_rects, List dst_rects);
static void print_area_of_rect(CvRect rect);
static void get_plate_image(IplImage * src_img, List  rects);

#define SCALE_MAX 4.5
#define SCALE_MIN 2.5
#define AREA_MAX 620
#define AREA_MIN 200
#define FILENAME_LEN 50


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

void get_location(IplImage * img_car)
{
	List rects = create_list();
	List rects_final = create_list();
	IplImage * img_after_preprocess = cvLoadImage("img_after_preprocess.bmp", -1);

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

	/*获得车牌图像*/
	get_plate_image(img_car, rects_final->next);

}

/*功能:找出图像中的所有矩形轮廓
 输入:预处理后的图像, 容纳矩形数据结构的List表,CvMemStorage容器, contours
 输出:容纳有CvRect类型矩形的List表

 */

void get_contour_rect(IplImage * src_img, List  rects, CvMemStorage * storage, CvSeq * contours)
{
	IplImage * temp_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	temp_img = cvCloneImage(src_img);
	
	/*找到所有轮廓,并存在容器storage中*/
	cvFindContours(temp_img, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	/*从storage中筛选出矩形轮廓*/
	while (contours != NULL) {
		push_back(rects, cvBoundingRect(contours, 0));
		rects = rects->next;
		contours = contours->h_next;
	}
}

/*画出之前找到的所有矩形形状的位置*/
void draw_contour_rect(IplImage * src_img, List  rects)
{
	if (rects == NULL) {
		fprintf(stderr, "rects is NULL!\n");
		exit(-1);
	}

	IplImage * temp_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	cvNamedWindow("img_with_rect");

	temp_img = cvCloneImage(src_img);
	if (temp_img == NULL) {
		fprintf(stderr, "temp_img is NULL!\n");
		exit(-1);
	}
	while (rects != NULL) {
		cvRectangle(temp_img, cvPoint(rects->item.x, rects->item.y), cvPoint(rects->item.x + rects->item.width, rects->item.y + rects->item.height), CV_RGB(0xbF, 0xbd, 0xab), 2, 8, 0);
		rects = rects->next;
	}
	cvShowImage("img_with_rect", temp_img);
	cvWaitKey(0);
}

/*通过形状比例筛选出满足形状比例的矩形*/
void filter_rect_by_shape(List src_rects, List dst_rects)
{
	while (src_rects != NULL) {
		double scale = 1.0 * (src_rects->item.width) / (src_rects->item.height);
		double area_of_rect = 1.0 * (src_rects->item.width) * (src_rects->item.height);

		if (scale <= SCALE_MAX && scale >= SCALE_MIN /*&& area_of_rect <= AREA_MAX && area_of_rect >= AREA_MIN*/) {
			push_back(dst_rects, src_rects->item);
			print_area_of_rect(dst_rects->next->item);
			dst_rects = dst_rects->next;
		}
		src_rects = src_rects->next;
	}
}



/*打印出矩形的面积*/
void print_area_of_rect(CvRect rect)
{
	printf("the area of this rect is %d\n", rect.width * rect.height);
}

/*获得车牌图像*/
void get_plate_image(IplImage * src_img, List  rects)
{
	IplImage * plate_img;
	int rect_count = 0;
	char filename[FILENAME_LEN];
	int i = 0;

	while (rects != NULL) {
		i++;
		sprintf(filename, "plate_img%d.bmp", rect_count);
		cvSetImageROI(src_img, cvRect(rects->item.x, rects->item.y, rects->item.width, rects->item.height));
		plate_img = cvCreateImage(cvSize(rects->item.width, rects->item.height), src_img->depth, src_img->nChannels);
		
		cvCopy(src_img, plate_img, 0);
		cvSaveImage(filename, plate_img);
		rect_count++;
		cvResetImageROI(src_img);
		rects = rects->next;
		if (i > 20) {
			break;
		}
	}
}


/*自定义膨胀腐蚀操作,在循环中使用可以增加定位准确度*/
void dilate_erode_x(IplImage * img_after_threshold, IplImage * img_final) {
	/*自定义1*3的核进行X方向的膨胀腐蚀*/
	IplConvKernel* kernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);
	cvDilate(img_after_threshold, img_final, kernal, 3);//X方向膨胀连通数字
	cvErode(img_final, img_final, kernal, 1);//X方向腐蚀去除碎片
	cvDilate(img_final, img_final, kernal, 3);//X方向膨胀回复形态
}

void dilate_erode_y(IplImage * img_final) {
	/*自定义3*1的核进行Y方向的膨胀腐蚀*/
	IplConvKernel* kernal = cvCreateStructuringElementEx(1, 3, 0, 1, CV_SHAPE_RECT);
	cvErode(img_final, img_final, kernal, 3);// Y方向腐蚀去除碎片
	cvDilate(img_final, img_final, kernal, 3);//回复形态
}



