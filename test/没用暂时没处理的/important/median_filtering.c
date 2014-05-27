#include "cv.h"
#include "highgui.h"

void median_filtering(IplImage * img);
void show_image_value_one(IplImage * img);
void create_image(IplImage * img);
int compare(const void * num1, const void * num2);


int main(int argc, char * argv[])
{
	IplImage * test_img;
	test_img = cvCreateImage(cvSize(5, 5), IPL_DEPTH_8U, 1);
	create_image(test_img);

	show_image_value_one(test_img);

	median_filtering(test_img);
	show_image_value_one(test_img);

	return 0;
}

void median_filtering(IplImage * img)
{

	int i, j;
	int a00, a01, a02;
	int a10, a11, a12;
	int a20, a21, a22;
	int a[9];


	for (i = 1; i < img->height - 1; i ++) {

		unsigned char * row0 = (unsigned char *)(img->imageData + (i - 1) * img->widthStep);
		
		unsigned char * row1 = (unsigned char *)(img->imageData + (i + 0) * img->widthStep);
		
		unsigned char * row2 = (unsigned char *)(img->imageData + (i + 1) * img->widthStep);


		for (j = 1; j < img->width - 1; j++) {
			a[0] = a00 = row0[j - 1];
			a[1] = a01 = row0[j + 0];
			a[2] = a02 = row0[j + 1];

			a[3] = a10 = row1[j - 1];
			a[4] = a11 = row1[j + 0];
			a[5] = a12 = row1[j + 1];

			a[6] = a20 = row2[j - 1];
			a[7] = a21 = row2[j + 0];
			a[8] = a22 = row2[j + 1];
	
//			printf("(%d %d %d)\n (%d %d %d)\n (%d %d %d)\n\n", a00, a01, a02, a10,a11,a12,a20,a21,a22);

			qsort(a, 9, sizeof(int), compare);
			row1[j] = a[4];
//			printf("%d\n",a[4]);
		}
		puts("\n");
	}
}

void show_image_value_one(IplImage * img)
{
	int i, j;
	FILE * fp_value;

	fp_value = fopen("value.txt", "a");

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width; j++) {
			fprintf(fp_value, "%d ", prow[j]);
		}
		fprintf(fp_value, "\n");

	}

	fprintf(fp_value, "\n");
	fclose(fp_value);
}

void create_image(IplImage * img)
{
	FILE * fp_test = fopen("test.txt", "r");

	int i, j;
	int count;
	fscanf(fp_test, "%d", &count);;

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
		for (j = 0; j < img->width; j++){
			prow[j] = count;	
			fscanf(fp_test, "%d", &count);;
		}

	}
}

int compare(const void * num1, const void * num2)
{
	return *((int *)num1) - *(int *)num2;
}
