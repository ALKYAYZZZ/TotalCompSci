all: My_genvec.o genmain.o
	c++ -std=c++11 genmain.o -o genmain
genmain.o: genmain.cpp My_genvec.h
	c++ -std=c++11 -c -g genmain.cpp
clean:
	rm *.o genmain