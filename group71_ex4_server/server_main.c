
#include "Server.h"

int main(int argc, char* argv[])
{
	// Check if enough params.
	if(argc == 2)
	{
		MainServer(argv[1]);
	}
	else
	{
		printf("not enough parameters to pass.\n");
	}
}