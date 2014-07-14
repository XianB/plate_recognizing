#include "include/plate.h"
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
	if (src_rects == NULL) {
		fprintf(stderr, "src_rects is empty!\n");
		exit(-1);
	}
	while (src_rects != NULL) {
		double scale = 1.0 * (src_rects->item.width) / (src_rects->item.height);
		double area_of_rect = 1.0 * (src_rects->item.width) * (src_rects->item.height);

		/*车牌有固定的形状比例以及大小比例,先按这个粗略提取出车牌位置*/
		if (scale <= SCALE_MAX && scale >= SCALE_MIN && area_of_rect <= AREA_MAX && area_of_rect >= AREA_MIN) {
			push_back(dst_rects, src_rects->item);
//			print_area_of_rect(dst_rects->next->item);
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


