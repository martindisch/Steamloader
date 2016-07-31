all: steamloader

steamloader: steamloader.o
	gcc steamloader.o -o steamloader

steamloader.o: steamloader.c
	gcc -c steamloader.c

clean:
	rm *.o steamloader
