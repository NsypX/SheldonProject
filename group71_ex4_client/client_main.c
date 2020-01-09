
#include "Client.h"
#include "string.h"
#include "MessegeHead.h"

int main(int argc, char* argv[])
{	
	// Check if got all arguments.
	if (argc == 4)
	{
		
		// run client.
		if (strlen(argv[3]) > MAX_NAME_SIZE)
		{
			printf("Name isnt valid. going down.");
		
		}
		else
		{
			MainClient(argv[1],argv[2], argv[3]);
		}
	}
	else
	{
		printf("not enough parameters to pass.\n");
	}
}