IplImage *  binarize_the_image(IplImage * img)
{
	int threshold = 120;
	int i, j;

//	threshold = get_threshold_for_binarization(img);
	
	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);

		for (j = 0; j < img->width; j++) {
			if (prow[j] > threshold)
			  prow[j] = 255;
			else
			  prow[j] = 0;
		}

	}

	return img;

}


//这里先使用自适应取阈值法
//事实显示自适应取阈值法并不是特别有效！

int  get_threshold_for_binarization(IplImage * img)
{
	int i, j;
	int threshold;

	int max = -1, min = 257;

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);

		for (j = 0; j < img->width; j++) {
			if (prow[j] > max)
			  max = prow[j];
			if (prow[j] < min)
			  min = prow[j];
		}

	}
	
	printf("the max/min number of this image is: %d %d\n ", max, min);

	threshold = max - (max - min) / 3;
	printf("the threshold is %d\n", threshold);
	return threshold;

}




