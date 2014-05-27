#include <math.h>

void Mysobel(IplImage * gray_img, IplImage * sobel_img)
{
	unsigned char a00, a01, a02;
	unsigned char a10, a11, a12;
	unsigned char a20, a21, a22;
	int ux,uy;


	//边缘几个点无法求导，忽略啦,所以从1开始
	for (int i = 1; i < gray_img->height - 1; i++) {
		unsigned char * prow = (unsigned char *)(gray_img->imageData + (i - 1) * gray_img->widthStep);

		unsigned char * prow1 = (unsigned char *)(gray_img->imageData + (i) * gray_img->widthStep);

		unsigned char * prow2 = (unsigned char *)(gray_img->imageData + (i + 1) * gray_img->widthStep);

		unsigned char * srow = (unsigned char *)(sobel_img->imageData + (i) * sobel_img->widthStep);



		for (int j = 1; j < gray_img->width - 1; j++){
			a00 = prow[j + 0];
			a01 = prow[j + 1];
			a02 = prow[j + 2];

			a10 = prow1[j + 0];
			a11 = prow1[j + 1];
			a12 = prow1[j + 2];

			a20 = prow2[j + 0];
			a21 = prow2[j + 1];
			a22 = prow2[j + 2];

			ux = a20 * (1) + a21 * (2) + a22 * (1) + (a00 * (-1) + a01 * (-2) + a02 * (-1));
			uy = a02 * (1) + a12 * (2) + a22 * (1) + a00 * (-1) + a10 * (-2) + a20 * (-1);
			srow[j + 1] = (int)sqrt(ux * ux + uy * uy);
//			printf("%d ", srow[j + 1]);
		}
//		printf("\n");
	}
}

