#ifndef _PLATE_H_
#define _PLATE_H_

#include "cv.h"
#include "highgui.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCALE_MAX 4.5
#define SCALE_MIN 2.5
#define AREA_MAX 15000
#define AREA_MIN 3000
#define FILENAME_LEN 50

void preprocess_car_img(IplImage * car_img);
List get_location(IplImage * img_car);
void get_plate_image(IplImage * img_car, List rects);
void resize_image(IplImage * img_to_resize, IplImage * img_after_resize, float scale);
void preprocess_plate_image(IplImage * img_after_resize);

void get_contour_rect(IplImage * src_img, List  rects, CvMemStorage * storage, CvSeq * contours);
void draw_contour_rect(IplImage * src_img, List  rects);
void filter_rect_by_shape(List src_rects, List dst_rects);
void print_area_of_rect(CvRect rect);


void remove_border_ul(IplImage * img_plate);
void get_character(IplImage * img);

int character_recognizing(IplImage * img_char);

#endif
