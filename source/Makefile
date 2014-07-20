plate:main.o preprocess.o get_location.o get_plate_image.o List.o resize_image.o preprocess_plate_image.o find_contours.o get_character.o character_recognizing.o include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` main.o preprocess.o get_location.o get_plate_image.o List.o resize_image.o preprocess_plate_image.o find_contours.o get_character.o character_recognizing.o -o plate `pkg-config opencv --libs`
main.o: main.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c main.c -o main.o `pkg-config opencv --libs`
preprocess.o: preprocess.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c preprocess.c -o preprocess.o `pkg-config opencv --libs`
get_location.o: get_location.c List.o find_contours.o include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c get_location.c List.o find_contours.o -o get_location.o `pkg-config opencv --libs`
get_plate_image.o: get_plate_image.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c get_plate_image.c -o get_plate_image.o `pkg-config opencv --libs`
List.o: List.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c List.c -o List.o `pkg-config opencv --libs`
resize_image.o: resize_image.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c resize_image.c -o resize_image.o `pkg-config opencv --libs`
preprocess_plate_image.o: preprocess_plate_image.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c preprocess_plate_image.c -o preprocess_plate_image.o `pkg-config opencv --libs`
find_contours.o:find_contours.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c find_contours.c -o find_contours.o `pkg-config opencv --libs`
get_character.o:get_character.c find_contours.o include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c get_character.c find_contours.o -o get_character.o `pkg-config opencv --libs`
character_recognizing.o: character_recognizing.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c character_recognizing.c -o character_recognizing.o `pkg-config opencv --libs`


clean:
	rm *.o plate *.bmp *.png

clean_image:
	rm *.bmp *.png

clean_obj:
	rm *.o 

