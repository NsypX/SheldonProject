#include "DefenitionsForServerClient.h"

void errorPrinter(int exitCode)
{
	if (exitCode == GENERAL_ERROR)
	{
		printf(GENERAL_ERROR_MSG);
	}
	else if (exitCode == DISCONNECTED_FROM_SERVER)
	{
		printf(DISCONNECT_FROM_SERVER_MSSG);
	}
	else if (exitCode == INPUT_ERROR)
	{
		printf(INPUT_ERROR_MSSG);
	}
	else if (exitCode == MALLOC_ERROR)
	{
		printf(MALLOC_ERROR_MSG);
	}
	else if (exitCode == ERROR_IN_CONNECTION)
	{
		printf(ERROR_IN_CONNECTION_ERROR);
	}
	else if (exitCode == FILE_READ_ERROR)
	{
		printf(FILE_READ_ERROR_MSG);
	}
	else if (exitCode == NO_PARAM_ERROR)
	{
		printf(NO_PARAM_ERROR_MSG);
	}
	else if (exitCode == THREAD_HANDLE_ERROR)
	{
		printf(THREAD_HANDLE_ERROR_MSG);
	}
	else if (exitCode == MUTEX_ERROR)
	{
		printf(MUTEX_ERROR_MSG);
	}
	else if (exitCode == THREAD_WAIT_ERROR)
	{
		printf(THREAD_WAIT_ERROR_MSG);
	}
	else if (exitCode == THREAD_TERMINATE_ERROR)
	{
		printf(THREAD_TERMINATE_ERROR_MSG);
	}
	else if (exitCode == THREAD_ERROR)
	{
		printf(THREAD_ERROR_MSG);
	}
	else if (exitCode < 0)
	{
		printf(THREAD_ERROR_MSG);
	}
}
