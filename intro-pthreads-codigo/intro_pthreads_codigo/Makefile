CC=gcc
CFLAGS=-O0 -pthread
EJECUS=forks pthreads holamundo1 holamundo2 holamundo3 mtgz

.PHONY = all clean new

all : $(EJECUS)

clean :
	rm -f $(EJECUS)

new : clean all

mtgz : mtgz.c
	$(CC) $(CFLAGS) $^ -o $@ -lz