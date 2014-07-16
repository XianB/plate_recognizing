#include "../include/plate.h"
#include <assert.h>

/*功能:对标准字符找到外接矩形的图像,并且进行二值化,为接下来的像素匹配做好准备
 输入:标准字符
 输出:二值化后并且大小为外接矩阵的图像.bmp
 */

void template_character(IplImage * img, char * filename);
void filter_rect_by_area(List src_rects, List dst_rects);

int main(int argc, char * argv[])
{
	IplImage * number_img;
	number_img = cvLoadImage(argv[1]);
	if (number_img == NULL) {
		fprintf(stderr, "Can not load image file.\n");
		exit(-1);
	}

	template_character(number_img, argv[1]);
}

void template_character(IplImage * img, char * filename)
{
	IplImage * img_gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage * img_final = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contours = NULL;
	List rects = create_list();
	List rects_final = create_list();

	cvCvtColor(img, img_gray, CV_RGB2GRAY);
	cvThreshold(img_gray, img_final, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	get_contour_rect(img_final, rects, storage, contours);
	filter_rect_by_area(rects->next, rects_final);

	draw_contour_rect(img_final, rects_final->next);
	cvSetImageROI(img_final, rects_final->next->item);
	IplImage * number = cvCreateImage(cvSize(rects_final->next->item.width, rects_final->next->item.height), img_final->depth, img_final->nChannels);
	cvCopy(img_final, number);
	cvSaveImage(filename, number);
}


/*挑选出矩形面积最大的那个矩形*/
void filter_rect_by_area(List src_rects, List dst_rects)
{
	if (src_rects == NULL) {
		fprintf(stderr, "src_rects is empty!\n");
		exit(-1);
	}

	List tmp_list = src_rects;
	int max_area = -1;

	while (tmp_list != NULL) {
		if (tmp_list->item.width * tmp_list->item.height > max_area) {
			max_area = tmp_list->item.width * tmp_list->item.height;

			dst_rects->next = (List)malloc(sizeof(Node));
			dst_rects->next->item = tmp_list->item;
		}
		tmp_list = tmp_list->next;
	}


#if 0
	while (tmp_list != NULL) {
		rects_count++;	
		tmp_list = tmp_list->next;
	}
	assert(rects_count >= 1);

	if (rects_count > 1) {
		while (src_rects != NULL) {
			double area_of_rect = 1.0 * (src_rects->item.width) * (src_rects->item.height);

			/*车牌有固定的形状比例以及大小比例,先按这个粗略提取出车牌位置*/
			if (area_of_rect > (total_area / 4)) {
				push_back(dst_rects, src_rects->item);
				dst_rects = dst_rects->next;
			}
			src_rects = src_rects->next;
		}
	} else {
		push_back(dst_rects, src_rects->item);
	}
#endif
}


