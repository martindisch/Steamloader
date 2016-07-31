all: steamloader

steamloader: steamloader.o steamaccess.o curlpost.o
	gcc steamloader.o steamaccess/steamaccess.o curlpost/curlpost.o -o steamloader -l curl

steamloader.o: steamloader.c
	gcc -c steamloader.c

steamaccess.o: steamaccess/steamaccess.c steamaccess/steamaccess.h
	gcc -c steamaccess/steamaccess.c -o steamaccess/steamaccess.o

curlpost.o: curlpost/curlpost.c
	gcc -c curlpost/curlpost.c -o curlpost/curlpost.o

clean:
	rm -f *.o */*.o steamloader
