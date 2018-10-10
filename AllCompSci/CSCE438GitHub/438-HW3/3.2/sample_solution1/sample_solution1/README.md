
Compile the code using the provided makefile:

    make

To clear the directory (and remove .txt files):
   
    make clean

First, run the routing server on port 5000 with the following command:

    ./tsr 5000
    
The routing server runs on port 4000 by default, but can be run on another port by passing it in as a parameter.

To run the message server on port 3010:
    
    ./run.sh -h host_addr:host_port -p 3010

The parameter passed to the -h flag must have the format host_addr:host_port, where host_addr is the IP address of the routing server (can be found with `ifconfig -a` on the routing server), and host_port is the port that the routing server was initialized with.

To run the client  

    ./tsc -h host_addr -p host_port -u user1

Host_addr and host_port are for the routing server as previously mentioned above.

NOTE: This system can sustain multiple message server failures (i.e. one server goes down, then comes back up later, then a different server goes down), though after the first server failure it may drop the first message sent by each client before resuming normal communications.
