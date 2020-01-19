/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - client_main.
   Description -  this is the main server function, check you have all args 
				  and run the server/
*/

#include "Server.h"
#include "LeaderBoarrd.h"


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