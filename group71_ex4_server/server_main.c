/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - client_main.
   Description -  this is the main server function, check you have all args 
				  and run the server/
*/
#define _CRT_MAP_ALLOC
#include "Server.h"
#include "LeaderBoarrd.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)

#define _ASSERTE(expr) ((void)0)
#endif

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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