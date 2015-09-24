
#include <stdio.h>
#include <stdlib.h>
#include "iterate.h"
#include "gauss.h"

static int start = 0;
static int end = 9;


int main (int argc, char* argv[])
{
	int total1, total2;

	

	if (argc == 3)
	{
		start	= atoi(argv[1]);
		end	= atoi(argv[2]);
	}


	

	total1 = iterate_get_sum (start, end);
	total2 = gauss_get_sum (start, end);


	

	if (total1 != total2)
	{
		printf ("Failure (%d != %d)!\n", total1, total2);
	}
	else
	{
		printf ("Success, sum[%d..%d] = %d\n", start, end, total1);
	}

	return 0;
}
