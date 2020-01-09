
#include "Client.h"
#include "MessegeHead.h"

int main(int argc, char* argv[])
{	
	// Check if got all arguments.
	if (argc == 4)
	{
		
		// run client.
		MainClient(argv[1],argv[2],argv[3]);
	}
	else
	{
		printf("not enough parameters to pass.\n");
	}
}