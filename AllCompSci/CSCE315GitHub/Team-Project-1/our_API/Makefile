CC= g++ -std=c++11
CFLAGS = -g

all:api

api:Table.o Record.o test.cpp
	$(CC) -o api Table.o Record.o test.cpp

Table.o: Table.h Table.cpp
	$(CC) -c Table.cpp

Record.o: Record.h Record.cpp
	$(CC) -c Record.cpp


clean:
	/bin/rm -f *.o api