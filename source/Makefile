main:main.o preprocess.o get_location.o get_plate_image.o List.o resize_image.o preprocess_plate_image.o include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` main.o preprocess.o get_location.o get_plate_image.o List.o resize_image.o preprocess_plate_image.o -o main `pkg-config opencv --libs`
main.o: main.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c main.c -o main.o `pkg-config opencv --libs`
preprocess.o: preprocess.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c preprocess.c -o preprocess.o `pkg-config opencv --libs`
get_location.o: get_location.c List.o include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c get_location.c List.o -o get_location.o `pkg-config opencv --libs`
get_plate_image.o: get_plate_image.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c get_plate_image.c -o get_plate_image.o `pkg-config opencv --libs`
List.o: List.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c List.c -o List.o `pkg-config opencv --libs`
resize_image.o: resize_image.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c resize_image.c -o resize_image.o `pkg-config opencv --libs`
preprocess_plate_image.o: preprocess_plate_image.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c preprocess_plate_image.c -o preprocess_plate_image.o `pkg-config opencv --libs`

clean:
	rm *.o main plate_img* img_after_preprocess.bmp

clean_image:
	rm *.bmp


