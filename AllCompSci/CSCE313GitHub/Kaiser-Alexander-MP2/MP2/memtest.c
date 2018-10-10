#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)
	int b = 0;
	int M = 0;
	//printf("getting command line arguments \n");
	int option = 0;
    while ((option = getopt(argc, argv,"b:s:")) != -1) {
        switch (option) {
             case 'b' : b = atoi(optarg);
				 if(b<=0){
					 printf("Invalid block size entered. Please enter a nonzero positive number for the block size.\n");
					 exit(0);
				 }
                 break;
             case 's' : M = atoi(optarg);
				 if(M<=0){
					 printf("Invalid memory size entered. Please enter a nonzero positive number for the memory size.\n");
					 exit(0);
				 }
                 break;
             default: break;
        }
    }
	if((b==0 && M==0)){
		b=128;
		M=524288;
	}
	printf("BLOCK SIZE: %d \nMEMORY SIZE: %d \n", b, M);
  
  init_allocator(b, M);

  ackerman_main();

   release_allocator();
}
