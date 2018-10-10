#---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman, Margaret Baxter                                   #
# Project: CSCE-313 Machine Problem #1                                         #
#                                                                              #
# File: ./MP1/Makefile                                                         #
# ---------------------------------------------------------------------------- #

CC=g++
STD=-std=c++11
CFLAGS=

all: main.o linked_list.o
	$(CC) $(STD) -o List_1 main.o linked_list.o
	
main.o: main.cpp
	$(CC) $(STD) -c -g main.cpp
	
linked_list.o: linked_list.cpp linked_list.h
	$(CC) $(STD) -c -g linked_list.cpp linked_list.h
	
clean:
	rm -rf *o List_1 *.gch
