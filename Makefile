all: steamloader

steamloader: steamloader.o steamaccess.o libcurlpost.a libjsmn.a
	gcc steamloader.o steamaccess/steamaccess.o curlpost/libcurlpost.a jsmn/libjsmn.a -o steamloader -lcurl

steamloader.o: steamloader.c
	gcc -c steamloader.c

steamaccess.o: steamaccess/steamaccess.c steamaccess/steamaccess.h
	gcc -c steamaccess/steamaccess.c -o steamaccess/steamaccess.o

libcurlpost.a: curlpost.o
	ar rc curlpost/libcurlpost.a curlpost/curlpost.o

curlpost.o: curlpost/curlpost.c
	gcc -c curlpost/curlpost.c -o curlpost/curlpost.o

libjsmn.a: jsmn.o
	ar rc jsmn/libjsmn.a jsmn/jsmn.o

jsmn.o: jsmn/jsmn.c jsmn/jsmn.h
	gcc -c jsmn/jsmn.c -o jsmn/jsmn.o

clean:
	rm -f *.o */*.o */*.a steamloader
