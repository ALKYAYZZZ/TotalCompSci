# makefile

all: memtest

ackerman.o: ackerman.c my_allocator.h
	gcc -c -g ackerman.c

my_allocator.o : my_allocator.c my_allocator.h
	gcc -c -g my_allocator.c

memtest.o : memtest.c ackerman.h
	gcc -c -g memtest.c

memtest: memtest.o ackerman.o my_allocator.o
	gcc -o memtest memtest.o ackerman.o my_allocator.o -lm

clean:
	rm *.o memtest