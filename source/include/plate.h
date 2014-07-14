#ifndef _PLATE_H_
#define _PLATE_H_

#include "cv.h"
#include "highgui.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

#define SCALE_MAX 4.5
#define SCALE_MIN 2.5
#define AREA_MAX 10020
#define AREA_MIN 800
#define FILENAME_LEN 50

void preprocess_car_img(IplImage * car_img);
List get_location(IplImage * img_car);
void get_plate_image(IplImage * img_car, List rects);
void resize_image(IplImage * img_to_resize, IplImage * img_after_resize, float scale);
void preprocess_plate_image(IplImage * img_after_resize);

#endif
