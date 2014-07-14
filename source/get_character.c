#include "include/plate.h"

/*
	功能是获得最后一个车牌字符
 */
static void find_last_character(IplImage * img, List rects);

void get_character(IplImage * img) 
{
	img = cvLoadImage("img_plate_after_preprocess.bmp", -1);
	List rects = create_list();
	List rects_final = create_list();

	if (img == NULL) {
		fprintf(stderr, "error in openning file.\n");
		exit(-1);
	}

	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contours = NULL;

	remove_border_ul(img);
	img = cvLoadImage("img_after_border_removed.bmp", -1);

	get_contour_rect(img, rects, storage, contours);
	filter_rect_by_area(rects, rects_final, img->width * img->height);
	draw_contour_rect(img, rects_final->next);
	find_last_character(img, rects_final->next);
}

/*消除上下边界*/
void remove_border_ul(IplImage * img_plate)
{
	int i = 0, j = 0;
	/*这两个变量分别为上下边界的高度*/
	int up_bound = -1, low_bound = -1;
	int white_to_black = 0;
	int black_to_white = 0;
	/*从i从0 到高度一半进行遍历,进行投影,找到上边界*/
	for (i = 0; i < (img_plate->height) / 2; i = i + 3) {

		unsigned char * prow = (unsigned char *)(img_plate->imageData + i * img_plate->widthStep);
		white_to_black = 0;
		black_to_white = 0;

		/*记录下每一行的black_to_white和w_to_b的个数*/
		for (j = 0; j < img_plate->width; j = j + 3) {
			if (prow[j] == 0 && prow[j + 3] == 255) {
				black_to_white++;
			} else if (prow[j] == 255 && prow[j + 3] == 0) {
				white_to_black++;
			}
		}

		if (black_to_white >= 6 && white_to_black >= 6 && up_bound < 0) {
			up_bound = i;
		} else if (black_to_white < 6 && white_to_black < 6 && up_bound > 0) {
			up_bound = -1;
		}
	}

	/*i从最底端到高度的一半进行遍历*/
	for (i = img_plate->height - 1; i > (img_plate->height) / 2; i = i - 3) {
		unsigned char * prow = (unsigned char *)(img_plate->imageData + i * img_plate->widthStep);
		white_to_black = 0;
		black_to_white = 0;

		/*记录下每一行的black_to_white和w_to_b的个数*/
		for (j = 0; j < img_plate->width; j = j + 3) {
			if (prow[j] == 0 && prow[j + 3] == 255) {
				black_to_white++;
			} else if (prow[j] == 255 && prow[j + 3] == 0) {
				white_to_black++;
			}
		}

		if (black_to_white >= 6 && white_to_black >= 6 && low_bound < 0) {
			low_bound = i;
		} else if (black_to_white < 6 && white_to_black < 6 && low_bound > 0) {
			low_bound = -1;
		}
	
	}
	
#if 0
	cvNamedWindow("img", 1);
	printf("up_bound is %d, low_bound is %d\n", up_bound, low_bound);

	/*画直线操作*/
	/*	void cvLine( CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color, int thickness=1, int line_type=8, int shift=0 );*/
	cvLine(img_plate, cvPoint(0, up_bound), cvPoint(img_plate->width - 3, up_bound), CV_RGB(0xbF, 0xfd, 0xba), 3, 8, 0);
	cvLine(img_plate, cvPoint(0, low_bound), cvPoint(img_plate->width - 3, low_bound), CV_RGB(0xbF, 0xfd, 0xba), 3, 8, 0);
	cvShowImage("img", img_plate);
	cvWaitKey(0);
#endif
	cvSetImageROI(img_plate, cvRect(0, up_bound, img_plate->width, low_bound - up_bound));
	IplImage * tmp_img = cvCreateImage(cvSize(img_plate->width, low_bound - up_bound), img_plate->depth, img_plate->nChannels);
	cvCopy(img_plate, tmp_img);
	cvSaveImage("img_after_border_removed.bmp", tmp_img);
	cvResetImageROI(img_plate);
}

/*通过大小比例筛选出满足形状比例的矩形*/
void filter_rect_by_area(List src_rects, List dst_rects, int total_area)
{
	if (src_rects == NULL) {
		fprintf(stderr, "src_rects is empty!\n");
		exit(-1);
	}
	while (src_rects != NULL) {
		double scale = 1.0 * (src_rects->item.width) / (src_rects->item.height);
		double area_of_rect = 1.0 * (src_rects->item.width) * (src_rects->item.height);

		/*车牌有固定的形状比例以及大小比例,先按这个粗略提取出车牌位置*/
		if (area_of_rect > (total_area / 15)) {
			push_back(dst_rects, src_rects->item);
			dst_rects = dst_rects->next;
		}
		src_rects = src_rects->next;
	}
}


/*功能:找到最后一个字符的图片
  参数:去除边界后的车牌图片,得到的矩形框列表
  思想:过找到rects中的各个矩形中左上角坐标最靠右边的那个矩形即可
 */
void find_last_character(IplImage * img, List rects)
{
	cvNamedWindow("last_character", 1);
	int max = -1;
	CvRect last;
	IplImage * last_character;
	/*遍历链表寻找x坐标最大的矩形*/
	while (rects != NULL) {
		if(rects->item.x > max) {
			max = rects->item.x;
			last = rects->item;
		}
		rects = rects->next;
	}
	cvSetImageROI(img, last);
	last_character = cvCreateImage(cvSize(last.width, last.height), img->depth, img->nChannels);

	cvCopy(img, last_character, 0);
	cvSaveImage("last_character.bmp", last_character);
	cvShowImage("last_character", last_character);
}











