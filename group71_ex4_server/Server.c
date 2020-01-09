/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
 This file was written for instruction purposes for the 
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering.
Last updated by Amnon Drory, Winter 2011.
 */
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#pragma region Includes

	#define _CRT_SECURE_NO_WARNINGS
	#define _WINSOCK_DEPRECATED_NO_WARNINGS

	#include "Server.h"


#pragma endregion

#pragma region Globals

	#define NUM_OF_WORKER_THREADS 2
	#define MAX_LOOPS 3
	#define SEND_STR_SIZE 35
	HANDLE ThreadHandles[NUM_OF_WORKER_THREADS];
	SOCKET ThreadInputs[NUM_OF_WORKER_THREADS];
	SockParams params[NUM_OF_WORKER_THREADS];
	int countLogedIn = 0;
	char IP_ADRESS[20];
	HANDLE gameSessionMutex;
	HANDLE waitForPlayerMutex;
	HANDLE gameHandlerSemaphore;

#pragma endregion

#pragma region Main Functions

	void increaseCountLogged(void)
	{
		countLogedIn++;
	}

	void closeHandles(void)
	{
		CloseHandle(gameSessionMutex);
		CloseHandle(waitForPlayerMutex);
		CloseHandle(gameHandlerSemaphore);
	}

	void MainServer(char* ip)
	{
		cleanNamesList();
		strcpy(IP_ADRESS, ip);
		int Ind;
		int Loop;
		SOCKET MainSocket = INVALID_SOCKET;
		unsigned long Address;
		SOCKADDR_IN service;
		int bindRes;
		int ListenRes;

		gameSessionMutex = CreateMutex(NULL, FALSE, NULL);
		waitForPlayerMutex = CreateMutex(NULL, FALSE, NULL);
		gameHandlerSemaphore = CreateSemaphore(NULL,0,CLIENT_AMOUNT,NULL);
		
		if (gameSessionMutex == NULL)
		{
			goto server_defaul_clean;
		}

		if(waitForPlayerMutex == NULL)
		{
			CloseHandle(gameSessionMutex);
			goto server_defaul_clean;
		}

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

		if (StartupRes != NO_ERROR)
		{
			printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
			// Tell the user that we could not find a usable WinSock DLL.                                  
			return;
		}

		// Create a socket.    
		MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (MainSocket == INVALID_SOCKET)
		{
			printf("Error at socket( ): %ld\n", WSAGetLastError());
			goto server_cleanup_1;
		}

		Address = inet_addr(IP_ADRESS);
		if (Address == INADDR_NONE)
		{
			printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
				IP_ADRESS);
			goto server_cleanup_2;
		}

		service.sin_family = AF_INET;
		service.sin_addr.s_addr = Address;
		service.sin_port = htons(SERVER_PORT);


		// Call the bind function, passing the created socket and the sockaddr_in structure as parameters. 
		// Check for general errors.
		bindRes = bind(MainSocket, (SOCKADDR*)&service, sizeof(service));
		if (bindRes == SOCKET_ERROR)
		{
			printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
			goto server_cleanup_2;
		}

		// Listen on the Socket.
		ListenRes = listen(MainSocket, SOMAXCONN);
		if (ListenRes == SOCKET_ERROR)
		{
			printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
			goto server_cleanup_2;
		}

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

	int waitGameSessionMutex(void)
	{
		int time = WaitForSingleObject(gameSessionMutex, INFINITE);
		return(time);
	}

	int releaseGameSessionMutex(void)
	{
		int time = ReleaseMutex(gameSessionMutex);
		return(time);
	}

	int waitFileMutex(void)
	{
		int time = WaitForSingleObject(waitForPlayerMutex, INFINITE);
		return(time);
	}

	int releasFileMutex(void)
	{
		int time = ReleaseMutex(waitForPlayerMutex);
		return(time);
	}

	int waitOtherPlayerMove(void)
	{
		int time = WaitForSingleObject(gameHandlerSemaphore, WAIT_FOR_CLIENT_TIME);
		return(time);
	}

	int waitOtherPlayerMoveINF(void)
	{
		int time = WaitForSingleObject(gameHandlerSemaphore, INFINITE);
		return(time);
	}

	int releaseOtherPlayerMove(void)
	{
		// Release one of the smaphores...
		int time = ReleaseSemaphore(gameHandlerSemaphore, 1, NULL);
		return(time);
	}

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
	//Service thread is the thread that opens for each successful client connection and "talks" to the client.
	static DWORD ServiceThread(SockParams *soc)
	{
		char SendStr[SEND_STR_SIZE];

		BOOL Done = FALSE;

		TransferResult_t RecvRes;
		SOCKET *t_socket = &soc->sd;
		int result = NO_ERROR1;

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

			if (result != NO_ERROR1)
			{
				//ERROR HANDLE	
			}

			free(AcceptedStr);
		}

		printf("Conversation ended.\n");
		closesocket(*t_socket);
		return 0;
	}

#pragma endregion



