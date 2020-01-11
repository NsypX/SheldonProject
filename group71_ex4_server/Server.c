/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - client_main.
   Description -  This file was written for instruction purposes for the 
				  course "Introduction to Systems Programming" at Tel-Aviv
				  University, School of Electrical Engineering.
				  Last updated by Amnon Drory, Winter 2011.
*/

#pragma region Includes

	#define _CRT_SECURE_NO_WARNINGS
	#define _WINSOCK_DEPRECATED_NO_WARNINGS

	#include "Server.h"


#pragma endregion

#pragma region Globals

	HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];
	HANDLE ExitHandle;
	SOCKET ThreadInputs[NUM_OF_WORKER_THREADS];
	SockParams params[NUM_OF_WORKER_THREADS];
	int countLogedIn = 0;
	int SERVER_PORT = 0;
	HANDLE gameSessionMutex;
	HANDLE waitForPlayerMutex;
	HANDLE gameHandlerSemaphore;
	int isToExit = FALSE_VAL;

#pragma endregion

#pragma region Main Functions

	/*
		Description - Close all the handles of the server.
		Parameters  -
		Returns     -
	*/
	void closeHandles(void)
	{
		CloseHandle(gameSessionMutex);
		CloseHandle(waitForPlayerMutex);
		CloseHandle(gameHandlerSemaphore);
	}

	/*
	Description - Runs the server threads.
	Parameters  - ip- the adress the server runs from.
	Returns     - 
	*/
	void MainServer(char* port)
	{
		// Remove if exist.
		remove(GAME_SESSION_LOC);

		// Initial stuff
		cleanNamesList();
		SERVER_PORT = atoi(port);
		int Ind;
		int Loop;
		SOCKET MainSocket = INVALID_SOCKET;
		unsigned long Address;
		SOCKADDR_IN service;
		int bindRes;
		int ListenRes;

		// Create mutex
		gameSessionMutex = CreateMutex(NULL, FALSE, NULL);
		waitForPlayerMutex = CreateMutex(NULL, FALSE, NULL);
		gameHandlerSemaphore = CreateSemaphore(NULL, 0, CLIENT_AMOUNT, NULL);

		// Mutex error handler.
		if (gameSessionMutex == NULL)
		{
			goto server_defaul_clean;
		}

		// Mutex error handler.
		if (waitForPlayerMutex == NULL)
		{
			CloseHandle(gameSessionMutex);
			goto server_defaul_clean;
		}

		// Mutex error handler.
		if (gameHandlerSemaphore == NULL)
		{
			CloseHandle(gameSessionMutex);
			CloseHandle(waitForPlayerMutex);
			goto server_defaul_clean;
		}

		// creating the list of cvs file.
		getLeaderInstanse();

		// Initialize Winsock.
		WSADATA wsaData;
		int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);

		// Check if went up.
		if (StartupRes != NO_ERROR)
		{
			printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
			return;
		}

		// Create a socket.    
		MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// Check for success
		if (MainSocket == INVALID_SOCKET)
		{
			printf("Error at socket( ): %ld\n", WSAGetLastError());
			goto server_cleanup_1;
		}

		// Set the adress.
		Address = inet_addr(IP_ADRESS);

		// Check if good adress.
		if (Address == INADDR_NONE)
		{
			printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
				IP_ADRESS);
			goto server_cleanup_2;
		}

		// Set the serverinfo.
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = Address;
		service.sin_port = htons(SERVER_PORT);


		// bind server to ip.
		bindRes = bind(MainSocket, (SOCKADDR*)&service, sizeof(service));
		if (bindRes == SOCKET_ERROR)
		{
			printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
			goto server_cleanup_2;
		}

		// Start listen to sockets.
		ListenRes = listen(MainSocket, SOMAXCONN);
		if (ListenRes == SOCKET_ERROR)
		{
			printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
			goto server_cleanup_2;
		}

		
		// Start the exit thread.
		ExitHandle = NULL;
		ExitHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExitThreadFunction, NULL, 0, NULL);

		// Initialize all thread handles to NULL, to mark that they have not been initialized
		for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
			ThreadHandles[Ind] = NULL;

		printf("Waiting for a client to connect...\n");

		// Waiting for  clients to connect.
		for (Loop = 0; Loop < MAX_LOOPS; Loop++)
		{
			SOCKET AcceptSocket = accept(MainSocket, NULL, NULL);

			// if accrepted.
			if (AcceptSocket == INVALID_SOCKET)
			{
				printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
				goto server_cleanup_3;
			}

			// debug print.
			printf("Client Connected.\n");

			// Find empty thread spot.
			Ind = FindFirstUnusedThreadSlot();

			if (Ind == NUM_OF_WORKER_THREADS) //no slot is available
			{
				// Im allowing 5 connection (to handle denie messages).
				closesocket(AcceptSocket); //Closing the socket, dropping the connection.
			}
			else
			{
				ThreadInputs[Ind] = AcceptSocket;
				params[Ind].sd = AcceptSocket;
				params[Ind].loc = Ind;
				ThreadHandles[Ind] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServiceThread, &params[Ind], 0, NULL);
			}
		}

	server_cleanup_3:
		{
			CleanupWorkerThreads();
			closeHandles();
		}

	server_cleanup_2:
		{
			if (closesocket(MainSocket) == SOCKET_ERROR)
				printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

			closeHandles();
		}

	server_cleanup_1:
		{
			closeHandles();
			if (WSACleanup() == SOCKET_ERROR)
				printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
		}

	server_defaul_clean:
		printf("Going down.");
	}

	/*
	Description - get string from the user.
	Parameters  - mssg- mssg for the user.
	Returns     - Error handle
	*/
	char* getStringFromUser()
	{
		// Set vars.
		char currMove = 'a';
		char* result = calloc(1, INPUT_TXT_SIZE);

		// Check malloc error.
		if (result == NULL)
		{
			return(NULL);
		}

		// Save begin of string.
		char* helper = result;

		// While std != end of line
		while (currMove != '\n')
		{
			// Get next char.
			currMove = getchar();

			// if end of std.
			if ((currMove == '\xff') || (currMove == '\251'))
			{
				// free result and return error.
				free(result);
				return(NULL);
			}

			if (currMove != '\n')
			{
				*helper = toupper(currMove);
			}

			helper++;
		}

		// set curr move = a.
		currMove = 'a';

		// set end of string.
		*helper = '\0';


		// return result.
		return(result);
	}

	/*
		Description - Publish count logged in to other modules.
		Parameters  - 
		Returns     - 
	*/
	void increaseCountLogged(void)
	{
		countLogedIn++;
	}

	/*
		Description - Find the first unused slot to run thread.
		Parameters  - 
		Returns     - The location desired.
	*/
	static int FindFirstUnusedThreadSlot()
	{
		int Ind;

		for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
		{
			if (ThreadHandles[Ind] == NULL)
				break;
			else
			{
				// poll to check if thread finished running:
				DWORD Res = WaitForSingleObject(ThreadHandles[Ind], 0);

				if (Res == WAIT_OBJECT_0) // this thread finished running
				{
					CloseHandle(ThreadHandles[Ind]);
					ThreadHandles[Ind] = NULL;
					break;
				}
			}
		}

		return Ind;
	}

	/*
	Description - Clean up threads in backround.
	Parameters  - 
	Returns     -
	*/
	static void CleanupWorkerThreads()
	{
		int Ind;

		for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
		{
			if (ThreadHandles[Ind] != NULL)
			{
				// poll to check if thread finished running:
				DWORD Res = WaitForSingleObject(ThreadHandles[Ind], INFINITE);

				if (Res == WAIT_OBJECT_0)
				{
					closesocket(ThreadInputs[Ind]);
					CloseHandle(ThreadHandles[Ind]);
					ThreadHandles[Ind] = NULL;
					break;
				}
				else
				{
					printf("Waiting for thread failed. Ending program\n");
					return;
				}
			}
		}
	}

	/*
	Description - Activate mutex outside module.
	Parameters  -
	Returns     -
	*/
	int waitGameSessionMutex(void)
	{
		int time = WaitForSingleObject(gameSessionMutex, INFINITE);
		return(time);
	}

	/*
	Description - Release mutex outside module.
	Parameters  -
	Returns     -
	*/
	int releaseGameSessionMutex(void)
	{
		int time = ReleaseMutex(gameSessionMutex);
		return(time);
	}

	/*
	Description - Activate mutex outside module.
	Parameters  -
	Returns     -
	*/
	int waitFileMutex(void)
	{
		int time = WaitForSingleObject(waitForPlayerMutex, INFINITE);
		return(time);
	}

	/*
	Description - Release mutex outside module.
	Parameters  -
	Returns     -
	*/
	int releasFileMutex(void)
	{
		int time = ReleaseMutex(waitForPlayerMutex);
		return(time);
	}

	/*
	Description - Activate mutex outside module timed.
	Parameters  -
	Returns     -
	*/
	int waitOtherPlayerMove(void)
	{
		int time = WaitForSingleObject(gameHandlerSemaphore, WAIT_FOR_CLIENT_TIME);
		return(time);
	}

	/*
	Description - Activate mutex outside module.
	Parameters  -
	Returns     -
	*/
	int waitOtherPlayerMoveINF(void)
	{
		int time = WaitForSingleObject(gameHandlerSemaphore, INFINITE);
		return(time);
	}

	/*
	Description - Release mutex outside module.
	Parameters  -
	Returns     -
	*/
	int releaseOtherPlayerMove(void)
	{
		// Release one of the smaphores...
		int time = ReleaseSemaphore(gameHandlerSemaphore, 1, NULL);
		return(time);
	}

	/*
	Description - Check if to approve client or to denie access.
	Parameters  -
	Returns     -
	*/
	int isLocationAvilableForClient()
	{
		if (countLogedIn > 1)
		{
			return(FALSE_VAL);
		}
		else
		{
			return(TRUE_VAL);
		}
	}

#pragma endregion

#pragma region Thread
	
	/*
		Description - Service thread runs in the back, getting messages and pharsing them.
		Parameters  - soc- socks params- the socket and its location in the array.
		Returns     -
	*/
	static DWORD ServiceThread(SockParams *soc)
	{
		char SendStr[SEND_STR_SIZE];

		BOOL Done = FALSE;

		TransferResult_t RecvRes;
		SOCKET *t_socket = &soc->sd;
		int result = NO_ERROR_VAL;

		while (!Done)
		{
			char *AcceptedStr = NULL;

			RecvRes = ReceiveString(&AcceptedStr, *t_socket);

			if (RecvRes == TRNS_FAILED)
			{
				countLogedIn--;
				printf("Service socket error while reading, closing thread.\n");
				closesocket(*t_socket);
				return 1;
			}
			else if (RecvRes == TRNS_DISCONNECTED)
			{
				if (result != SERVER_DENIE_CLIENT)
				{
					countLogedIn--;
				}

				printf("Connection closed while reading, closing thread.\n");
				closesocket(*t_socket);
				return 1;
			}
			else
			{
				result = pharseMessage(AcceptedStr, soc);
			}

			if (result != NO_ERROR_VAL)
			{
				//ERROR HANDLE	
			}

			free(AcceptedStr);
		}

		printf("Conversation ended.\n");
		closesocket(*t_socket);
		return 0;
	}

	/*
		Description - Waiting for exit to close program.
		Parameters  - 
		Returns     -
	*/
	static DWORD ExitThreadFunction(void)
	{	
		isToExit = FALSE_VAL;
		char* txt = getStringFromUser();

		while (strcmp(txt, "EXIT") != 0)
		{
			// Waiting for input from server
			txt = getStringFromUser();
		}


		isToExit = TRUE_VAL;
	}

#pragma endregion



