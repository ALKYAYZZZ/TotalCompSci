/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_2/main.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <getopt.h>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list2.h"

int main(int argc, char ** argv) 
{
	int c = 0;
	int b = 128;
	int M = b * 16;  	// so we have space for 16 items in the whole list
	int t = 4;			// 4 tiers and 4 items per tier

	while ((c = getopt (argc, argv, "b:s:t:")) != -1)
		switch (c)
		  {
		case 'b':
			b = atoi(optarg);
			break;
		case 's':
			M = atoi(optarg);
			break;
		case 't':
			t = atoi(optarg);
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
	
	char buf [1024];
	memset (buf, 1, 1024);		// set each byte to 1
	
	char * msg = "a sample message";
	
	// Instantiate the tiered list class
	linked_list2 *test_list = new linked_list2;
	
	test_list->Init(M, b, t); // initialize
	// test operations
	int testnums [] = {0, 1<<29 , (1<<29) + 5 , 50, (1<<30) + 5, (1<<30) - 500};
	int i = 0;
	// some sample insertions
	for (i=0; i< 6; i ++)
	{
		test_list->Insert (testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	test_list->PrintList();
	test_list->Insert(56,msg,20);
	test_list->Insert(1208420384,msg,15);
	test_list->Insert(120842038400000000,msg,15);
	test_list->Delete(0);
	
	test_list->PrintList();
	
	node* kv = test_list->Lookup (56);
	if (kv)
    {
		printf ("Key = %d, Value Len = %d, Value = %s\n", kv->key, kv->value_len, (char *)(kv + 1));
    }
		
	// end test operations	
	test_list->Destroy();
}
