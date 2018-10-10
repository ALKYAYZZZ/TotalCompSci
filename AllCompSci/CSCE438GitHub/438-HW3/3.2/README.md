# CSCE 438 HW3.1
<b>Description</b>\
The purpose of this assignment is to create a more reliable Social Network Service on multiple servers using gRPC. In the event
of a crashed server, the crash must be transparent to the client and the server must be restarted. Data must be persistent between all servers so that when the client switches servers in the background, no data is lost. Data that is processed while the server is crashed does not need to be persistent for this assignment.\
<b>Compilation</b>\
-To compile the program, navigate to the src folder and run the make command.\
<b>Running the Program</b>\
-To run the Routing Server, use the command ./routing_server -h<hostname> -p<portnumber>. To run the Available server run the command ./tsd -h<hostname> -p<portnumber> -r<routerhost> -t<routerport> -s<mpid>. To run the client use the command 
./tsc -h<hostname> -u<username> -p<portnumber>.

<b>Server Information</b>\
<b>Available Server:</b>  Send incoming msgs to other servers, if they are down keep track of what they missed (so we can send it when they come back online)\
<b>Routing Server:</b>  Keep track of host/port of all other servers, elect Available Server when needed (and tell it host/port of all other servers)

