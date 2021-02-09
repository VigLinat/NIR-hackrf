#include "my_getopt.h"
char* optarg;
int my_getopt(int argc, char** argv)
{
	static int i = 1;
	if ((i < argc) && (argc > 1)) 
	{
		if ( *argv[i] == '-' ) 
		{
			int res = *(argv[i] + 1);
			if (i == argc - 1) 
			{
				i = i + 1;
				return res;
			}
			if ( *argv[i + 1] == '-' ) 
			{
				i = i + 1;
				return res;
			}
			else 
			{
				optarg = argv[i + 1];
				i = i + 2;
				return res;
			}
		}	
	}
	else 
		return EOF;
	return EOF;
}
