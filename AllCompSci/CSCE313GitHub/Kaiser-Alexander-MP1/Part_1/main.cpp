/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"

int main(int argc, char ** argv) 
{
	int c = 0;
	int M = 512000;
	int b = 128;
	while ((c = getopt (argc, argv, "b:s:")) != -1)
		switch (c)
		  {
		  case 'b':
			b = atoi(optarg);
			break;
		  case 's':
			M = atoi(optarg);
			break;
		  case '?':
			if (optopt == 'c')
			  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
			  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
			  fprintf (stderr,
					   "Unknown option character `\\x%x'.\n",
					   optopt);
			return 1;
		  default:
			abort ();
		  }
	
	//int b = 128;
	//int M = b * 11;  // so we have space for 11 items
	
	char buf [1024];
	memset (buf, 1, 1024);		// set each byte to 1
	
	char * msg = "a sample message";
	
    linked_list* test_list = new linked_list();

	test_list->Init(M,b); // initialize
	
    // test operations
	int testnums [] = {100, 5, 200, 7, 39, 25, 400, 50, 200, 300};
	int i = 0;
	
    // some sample insertions
	for (i=0; i< 9; i ++)
	{
		test_list->Insert(testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	
    // This Insert should fail
    test_list->Insert (150, buf, 200);

	test_list->PrintList ();
	test_list->Delete (7);
	test_list->Delete (25);
	test_list->Insert (13, msg, strlen(msg)+1);		// insertion of strings, copies the null byte at the end
	test_list->Delete (55);
	//test_list->Insert (15, "test msg", 8);
	test_list->Delete (3);
	test_list->PrintList ();
	
	// a sample lookup operations that should return null, because it is looking up a non-existent number
	node* kv = test_list->Lookup (3);
	if (kv)
    {
		printf ("Key = %d, Value Len = %d\n", kv->key, kv->value_len);
    }

	// this look up  should succeed and print the string "a sample message"
	kv = test_list->Lookup (13);
	if (kv)
    {
		printf ("Key = %d, Value Len = %d, Value = %s\n", kv->key, kv->value_len, (char *)(kv + 1));
    }
	
	// end test operations	
	test_list->Destroy ();
	
}
