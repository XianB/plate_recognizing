#include "plate.hpp"

class Img{
	public:
		IplImage * pimg;
	public:
		Img(){
			pimg = NULL;
		}
		/*通过用参数图像来创建一个图像*/
		Img(Img * i, int depth = IPL_DEPTH_8U, int channels = 1){
			pimg = cvCreateImage(cvGetSize(i->pimg), depth, channels);
		}
		Img(CvSize size, int depth = IPL_DEPTH_8U, int channels = 1){
			pimg = cvCreateImage(size, depth, channels);
		}

		Img(char * img_name){
			pimg = cvLoadImage(img_name);
			if (pimg == NULL) {
				fprintf(stderr, "Error! Can not open image file!\n");
			}
		}

		Img(const Img &i) {
			pimg = cvCreateImage(cvGetSize(i.pimg), IPL_DEPTH_8U, 1);
		}

		~Img(){
			cvReleaseImage(&(this->pimg));	
		}

		void show_image(char * window_name){
			if (pimg == NULL) {
				fprintf(stderr, "Error! Can not open image file!\n");
			}
			cvNamedWindow(window_name);
			cvShowImage(window_name, this->pimg);
			cvWaitKey(0);
		}
};
