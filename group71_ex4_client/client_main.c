
#include "Client.h"
#include "MessegeHead.h"

int main(int argc, char* argv[])
{	
	if (argc == 4)
	{
		MainClient(argv[1],argv[2],argv[3]);
	}
	else
	{
		printf("not enough parameters to pass.\n");
	}
}