#include "../include/plate.h"

int main(int argc, char * argv[])
{
	IplImage * plate_img = cvLoadImage(argv[1], -1);
	IplImage * plate_img_after_preprocess;
	IplImage * last_character;
	if (plate_img == NULL) {
		fprintf(stderr, "Error in open file %s\n", argv[1]);
		exit(-1);
	}

	preprocess_plate_image(plate_img);

	plate_img_after_preprocess = cvLoadImage("img_plate_after_preprocess.bmp", -1);
	if (plate_img_after_preprocess == NULL) {
		fprintf(stderr, "Error in open file\n");
		exit(-1);
	}
	
	get_character(plate_img_after_preprocess);
	last_character = cvLoadImage("last_character.bmp", -1);

	if (last_character == NULL) {
		fprintf(stderr, "Error in open file\n");
		exit(-1);
	}

	character_recognizing(last_character);

	return 0;
}
