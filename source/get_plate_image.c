#include "include/plate.h"

/*获得车牌图像*/
void get_plate_image(IplImage * src_img, List  rects);

void get_plate_image(IplImage * src_img, List  rects)
{
	if (src_img == NULL || rects == NULL) {
		fprintf(stderr, "src_img or rects is NULL!\n");
		exit(-1);
	}

	IplImage * plate_img;
	int rect_count = 0;
	char filename[FILENAME_LEN];
	int i = 0;

	while (rects != NULL) {
		i++;
		sprintf(filename, "plate_img%d.bmp", rect_count);
		cvSetImageROI(src_img, rects->item);
//		cvSetImageROI(src_img, cvRect(rects->item.x, rects->item.y, rects->item.width, rects->item.height));
		plate_img = cvCreateImage(cvSize(rects->item.width, rects->item.height), src_img->depth, src_img->nChannels);
		
		cvCopy(src_img, plate_img, 0);
		cvSaveImage(filename, plate_img);
		rect_count++;
		cvResetImageROI(src_img);
		rects = rects->next;

		/*控制不能让车牌数量太多了*/
		if (i > 200) {
			break;
		}
	}
}


