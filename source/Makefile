main:main.o preprocess.o get_location.o include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` main.o preprocess.o get_location.o -o main `pkg-config opencv --libs`
main.o: main.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c main.c -o main.o `pkg-config opencv --libs`
preprocess.o: preprocess.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c preprocess.c -o preprocess.o `pkg-config opencv --libs`
get_location.o: get_location.c include/plate.h include/List.h
	g++ `pkg-config opencv --cflags` -c get_location.c -o get_location.o `pkg-config opencv --libs`

clean:
	rm main.o preprocess.o get_location.o main plate_img* img_after_preprocess.bmp


