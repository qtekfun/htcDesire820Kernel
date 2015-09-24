
#include <stdio.h>
#include <stdlib.h>
#include "iterate.h"


int iterate_get_sum (int min, int max)
{
	int i, total;

	total = 0;


	for (i = min; i <= max; i++)
	{

		if (total + i < total)
		{
			printf ("Error: sum too large!\n");
			exit (1);
		}

		

		total += i;
	}

	return total;
}
