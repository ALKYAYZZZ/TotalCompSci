# makefile

CC=g++
STD=-std=c++11
CFLAGS=

all: memtest

ackerman.o: ackerman.c 
	$(CC) $(STD) -c -g ackerman.c 

my_allocator.o : my_allocator.c
	$(CC) $(STD) -c -g my_allocator.c

memtest.o : memtest.c
	$(CC) $(STD) -c -g memtest.c

memtest: memtest.o ackerman.o my_allocator.o
	$(CC) $(STD) -o memtest memtest.o ackerman.o my_allocator.o

clean:
	rm -rf *o memtest *.gch