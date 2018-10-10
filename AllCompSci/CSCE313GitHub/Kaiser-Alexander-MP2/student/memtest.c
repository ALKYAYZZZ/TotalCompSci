/* ------------------------------------------------------------------------ */
/* Developer: Andrew Kirfman, Dr. Betatti                                   */
/* Project: CSCE-313 Machine Problem 2                                      */
/*                                                                          */
/* File: ./memtest.c                                                        */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* User Defined Includes                                                    */
/* ------------------------------------------------------------------------ */

#include "my_allocator.h"
#include "ackerman.h"

MyAllocator* my_alloc;

int main(int argc, char ** argv) 
{
	int c = 0;
	int s = 512000;
	int b = 128;
	while ((c = getopt (argc, argv, "b:s:")) != -1)
		switch (c)
		  {
		  case 'b':
			b = atoi(optarg);
			break;
		  case 's':
			s = atoi(optarg);
			break;
		  case '?':
			if (isprint (optopt))
			  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
			  fprintf (stderr,
					   "Unknown option character `\\x%x'.\n",
					   optopt);
			return 1;
		  default:
			abort ();
		  }
	my_alloc = new MyAllocator((unsigned int)b, (unsigned int)s);
	

	// Use getopt to grab command line arguments
	// from argv.  Use these arguments as inputs to your program
	//my_alloc->init_allocator(b, s);
	ackerman_main();
	/*
	Addr alloc_1 = my_alloc->my_malloc(128);
	Addr alloc_2 = my_alloc->my_malloc(128);
	Addr alloc_3 = my_alloc->my_malloc(128);
	Addr alloc_4 = my_alloc->my_malloc(128);
	Addr alloc_5 = my_alloc->my_malloc(128);
	Addr alloc_6 = my_alloc->my_malloc(128);
	my_alloc->print_array();
	my_alloc->my_free(alloc_2);
	my_alloc->print_array();
	my_alloc->my_free(alloc_4);
	my_alloc->print_array();
	my_alloc->my_free(alloc_6);
	my_alloc->print_array();
	*/
	/*
	Addr alloc_1 = my_alloc->my_malloc(157);
	my_alloc->my_free(alloc_1);
	my_alloc->print_array();
	Addr alloc_2 = my_alloc->my_malloc(599);
	my_alloc->print_array();
	Addr alloc_3 = my_alloc->my_malloc(4);
	my_alloc->print_array();
	my_alloc->my_free(alloc_3);
	
	Addr alloc_4 = my_alloc->my_malloc(1999);
	Addr alloc_5 = my_alloc->my_malloc(37);
	Addr alloc_6 = my_alloc->my_malloc(82);
	Addr alloc_7 = my_alloc->my_malloc(4392);
	Addr alloc_8 = my_alloc->my_malloc(2379);
	Addr alloc_9 = my_alloc->my_malloc(42681);
	Addr alloc_10 = my_alloc->my_malloc(32000);
	Addr alloc_11 = my_alloc->my_malloc(82);
	Addr alloc_12 = my_alloc->my_malloc(82);
	Addr alloc_13 = my_alloc->my_malloc(82);
	Addr alloc_14 = my_alloc->my_malloc(32000);
	my_alloc->my_free(alloc_8);
	
	
	my_alloc->print_array();
	*/
	
	my_alloc->release_allocator();
	
    std::cout << "Hello World!" << std::endl;

	return 0;
}
