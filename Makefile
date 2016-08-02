all: steamloader

steamloader: steamloader.o steamaccess.o libcurlpost.a libcurlloader.a libjsmn.a
	gcc steamloader.o steamaccess/steamaccess.o curlpost/libcurlpost.a curlloader/libcurlloader.a jsmn/libjsmn.a -o steamloader -lcurl

debug: steamloader.o steamaccess.o libcurlpost.a libcurlloader.a libjsmn.a
	gcc -g -O0 steamloader.o steamaccess/steamaccess.o curlpost/libcurlpost.a curlloader/libcurlloader.a jsmn/libjsmn.a -o steamloader -lcurl

steamloader.o: steamloader.c
	gcc -c steamloader.c

steamaccess.o: steamaccess/steamaccess.c steamaccess/steamaccess.h
	gcc -c steamaccess/steamaccess.c -o steamaccess/steamaccess.o

libcurlpost.a: curlpost.o
	ar rc curlpost/libcurlpost.a curlpost/curlpost.o

curlpost.o: curlpost/curlpost.c curlpost/curlpost.h
	gcc -c curlpost/curlpost.c -o curlpost/curlpost.o

libcurlloader.a: curlloader.o
	ar rc curlloader/libcurlloader.a curlloader/curlloader.o

curlloader.o: curlloader/curlloader.c curlloader/curlloader.h
	gcc -c curlloader/curlloader.c -o curlloader/curlloader.o

libjsmn.a: jsmn.o
	ar rc jsmn/libjsmn.a jsmn/jsmn.o

jsmn.o: jsmn/jsmn.c jsmn/jsmn.h
	gcc -c jsmn/jsmn.c -o jsmn/jsmn.o

clean:
	rm -f *.o */*.o */*.a steamloader
