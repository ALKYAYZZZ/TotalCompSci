OBJ = crsd.o chatroom.o
MAIN = crc.o
FLAGS = -pthread -g -std=c++11 -DCHAT_IP=\"127.0.0.1\" -DCHAT_PORT=5994
ALL: client server

chatroom.o: chatroom.cpp
	g++ $(FLAGS) -c chatroom.cpp

server: crsd.cpp chatroom.o message.hpp
	g++ $(FLAGS) crsd.cpp chatroom.o -o server -lpthread

client:	crc.cpp message.hpp
	g++ $(FLAGS) crc.cpp -o client

clean: 
	rm -f $(OBJ) $(MAIN) client server
