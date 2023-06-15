.PHONY: all cleanall

FLAGS :=-pthread -lrt

all:
	gcc -g semaphore.c -o semaphore $(FLAGS)
	gcc -g example.c -o example $(FLAGS)

cleanall:
	rm -f fsemaphore*
	rm -f *.o
