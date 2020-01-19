/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - client_main.
   Description - this file runs the program.
*/
#define _CRT_MAP_ALLOC
#include "Client.h"
#include "string.h"
#include <stdlib.h>
#include "DefenitionsForServerClient.h"
#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)

#define _ASSERTE(expr) ((void)0)
#endif

int main(int argc, char* argv[])
{	
	// Check if got all arguments.
	if (argc == 4)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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