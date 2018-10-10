# makefile

CC=g++
STD=-std=c++11
CFLAGS=

all: memtest

ackerman.o: ackerman.c 
	$(CC) $(STD) g++ -c -g ackerman.c -std=c++11

my_allocator.o : my_allocator.c
	$(CC) $(STD) g++ -c -g my_allocator.c -std=c++11

memtest.o : memtest.c
	$(CC) $(STD) g++ -c -g memtest.c -std=c++11

memtest: memtest.o ackerman.o my_allocator.o
	$(CC) $(STD) g++ -o memtest memtest.o ackerman.o my_allocator.o

clean:
	rm -rf *o memtest *.gch

