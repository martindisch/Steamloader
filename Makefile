all: steamloader

steamloader: steamloader.o steamaccess.o curlpost.o
	gcc steamloader.o steamaccess/steamaccess.o steamaccess/curlpost.o -o steamloader -l curl

steamloader.o: steamloader.c
	gcc -c steamloader.c

steamaccess.o: steamaccess/steamaccess.c steamaccess/steamaccess.h
	gcc -c steamaccess/steamaccess.c -o steamaccess/steamaccess.o

curlpost.o: steamaccess/curlpost.c
	gcc -c steamaccess/curlpost.c -o steamaccess/curlpost.o

clean:
	rm -f *.o steamaccess/*.o steamloader
