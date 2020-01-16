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
	HANDLE ClientHandle;
	SOCKET ThreadInputs[NUM_OF_WORKER_THREADS];
	SockParams params[NUM_OF_WORKER_THREADS];
	int countLogedIn = 0;
	int SERVER_PORT = 0;
	HANDLE gameSessionMutex;
	HANDLE waitForPlayerMutex;
	HANDLE gameHandlerSemaphore;
	int isToExit = FALSE_VAL;
	int Ind;
	SOCKET MainSocket = INVALID_SOCKET;
	char IP_ADRESS[MAX_IP_SIZE] = "127.0.0.1";

#pragma endregion

#pragma region Main Functions

	/*
		Description - Close all the handles of the server.
		Parameters  -
		Returns     -
	*/
	void closeHandles(void)
	{
		if (gameSessionMutex != NULL)
		{		
			CloseHandle(gameSessionMutex);
		}

		if (waitForPlayerMutex != NULL)
		{
			CloseHandle(waitForPlayerMutex);
		}

		if (gameHandlerSemaphore != NULL)
		{
			CloseHandle(gameHandlerSemaphore);
		}
	}

	/*
		Description - print all error from clients.
		Parameters  -
		Returns     -
	*/
	void printErrorFromClient(void)
	{
		for (int i = 0; i < NUM_OF_WORKER_THREADS; i++)
		{			
			// Get Exit Code 
			int exitcode = NO_ERROR;
			GetExitCodeThread(ThreadHandles[i], &exitcode);

			errorPrinter(exitcode);
		}
	}

	/*
		Description - delete space from ipconfig.
		Parameters  -
		Returns     -
	*/
	void deleteIpSpace(void)
	{
		int i = 1;
		for (i = 1; i < strlen(IP_ADRESS); i++)
		{
			
			IP_ADRESS[i - 1] = IP_ADRESS[i];
		}
		IP_ADRESS[i-1] = '\0';
	}

	/*
		Description - get the ip from ipconfig remain 127.0.0.1 as default.
		Parameters  -
		Returns     -
	*/
	char* getIpAdress(int* result)
	{
		// Remove file if exist.
		remove(IP_ADRESS_FILE);

		// Save ip config to out.txt
		system("ipconfig | findstr \"IPv4\" > out.txt");

		// Open the file.
		FILE* ipfile = fopen(IP_ADRESS_FILE, "r");
		char ip[MAX_IP_SIZE] = "";

		// Check if file open.
		if (ipfile == NULL)
		{
			*result = FILE_READ_ERROR;
		}
		else
		{
			// Read lines.
			while (fgets(ip, MAXCHAR, ipfile) != NULL)
			{	

				// Get first part.
				char* mssg = strtok(ip, ":");

				// If ipv4
				if (strcmp(mssg, IPCONFIG_STRING) == 0)
				{
					// Save ip.
					char* helper = strtok(NULL, ":");
					helper = strtok(helper, "\n");
					strcpy(IP_ADRESS, helper);
					deleteIpSpace();
				}
			}

			fclose(ipfile);
		}

		// Remove file.
		remove(IP_ADRESS_FILE);
	}

	/*
	Description - Runs the server threads.
	Parameters  - ip- the adress the server runs from.
	Returns     - 
	*/
	void MainServer(char* port)
	{
		// Clear screen.
		system("cls");
		// Remove if exist.
		remove(GAME_SESSION_LOC);		
		// Initial stuff
		cleanNamesList(); SERVER_PORT = atoi(port);	int Loop; unsigned long Address; SOCKADDR_IN service; int bindRes; int ListenRes; int result = NO_ERROR_VAL;		
		getIpAdress(&result);
		// Create mutex
		gameSessionMutex = CreateMutex(NULL, FALSE, NULL); waitForPlayerMutex = CreateMutex(NULL, FALSE, NULL); gameHandlerSemaphore = CreateSemaphore(NULL, 0, CLIENT_AMOUNT, NULL);
		
		// Mutex error handler.
		if ((gameSessionMutex == NULL) || (waitForPlayerMutex == NULL) || (gameHandlerSemaphore == NULL))
		{goto cleandHandles;}
	
		// creating the list of cvs file.
		getLeaderInstanse(&result);

		// Initialize Winsock.
		WSADATA wsaData;
		int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
		
		// Check if went up.
		if (StartupRes != NO_ERROR)
		{printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());  goto server_defaul_clean;}

		// Create a socket.    
		MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		
		if (MainSocket == INVALID_SOCKET)
		{
			printf("Error at socket( ): %ld\n", WSAGetLastError()); goto server_cleanup_1;
		}
		// Set the adress.		
		Address = inet_addr(IP_ADRESS);
		
		// Check if good adress.
		if (Address == INADDR_NONE)
		{
			printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",IP_ADRESS); goto server_cleanup_2;
		}

		// Set the serverinfo.
		service.sin_family = AF_INET; service.sin_addr.s_addr = Address; service.sin_port = htons(SERVER_PORT);
		
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
		
		// Initialize all thread handles to NULL, to mark that they have not been initialized
		for (Ind = 0; Ind < NUM_OF_WORKER_THREADS; Ind++)
			ThreadHandles[Ind] = NULL;

		// Start the exit thread.
		ExitHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExitThreadFunction, NULL, 0, NULL);;		
		ClientHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleClients, NULL, 0, NULL);

		int wait = WaitForSingleObject(ExitHandle, INFINITE);		
		if (wait != WAIT_OBJECT_0)
			result = THREAD_ERROR;	
		int exitcode = NO_ERROR;
		GetExitCodeThread(ExitHandle, &exitcode);		
		if (exitcode < 0)
			errorPrinter(exitcode);			
		wait = WaitForSingleObject(ClientHandle, INFINITE);				
		if (wait != WAIT_OBJECT_0)
			result = THREAD_ERROR;		
		GetExitCodeThread(ClientHandle, &exitcode);		
		if (exitcode < 0)
			errorPrinter(exitcode);
		
		freeLeaderInstanse(&result);
		errorPrinter(result);

	server_cleanup_3:
		{CleanupWorkerThreads();}
	server_cleanup_2:
		{if(MainSocket != NULL)
			{if (closesocket(MainSocket) == SOCKET_ERROR)
					printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());}}
	server_cleanup_1:
		{if (WSACleanup() == SOCKET_ERROR)
				printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());}
	cleandHandles:
		{closeHandles();}

	server_defaul_clean:
		{printf("Going down.");}
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
		Description - Closing 1 client.
		Parameters  -
		Returns     -
	*/
	static void closeCurrclient(int i)
	{
		// check if null and close.
		if (params[i].sd != NULL)
		{
			closesocket(params[i].sd);
			params[i].sd = NULL;
		}
	}

	/*
	Description - Closing all the clients.
	Parameters  -
	Returns     -
	*/
	static void closeallClient()
	{
		for (int i = 0; i < NUM_OF_WORKER_THREADS; i++)
		{
			closeCurrclient(i);
		}
	}

	void outCloseClient(int i)
	{
		closeCurrclient(i);
	}

	/*
	Description - Activate mutex outside module.
	Parameters  -
	Returns     -
	*/
	int waitGameSessionMutex(void)
	{
		int time = WaitForSingleObject(gameSessionMutex, INFINITE);

		if (time == WAIT_OBJECT_0)
		{
			return time;
		}
		else
		{
			return(THREAD_ERROR);
		}
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
		
		if (time == WAIT_OBJECT_0)
		{
			return time;
		}
		else
		{
			return(THREAD_ERROR);
		}
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
		
		if (time == WAIT_OBJECT_0)
		{
			return time;
		}
		else
		{
			return(THREAD_ERROR);
		}
	}

	/*
	Description - Activate mutex outside module.
	Parameters  -
	Returns     -
	*/
	int waitOtherPlayerMoveINF(void)
	{
		int time = WaitForSingleObject(gameHandlerSemaphore, INFINITE);
		
		if (time == WAIT_OBJECT_0)
		{
			return time;
		}
		else
		{
			return(THREAD_ERROR);
		}

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
				closesocket(*t_socket);
				return 1;
			}
			else if (RecvRes == TRNS_DISCONNECTED)
			{
				if (result != SERVER_DENIE_CLIENT)
				{
					countLogedIn--;
				}
				
				closesocket(*t_socket);
				return 1;
			}
			else
			{
				result = pharseMessage(AcceptedStr, soc);

				if (result < 0)
				{
					Done = TRUE;
				}

			}

			if (result != NO_ERROR_VAL)
			{
				//ERROR HANDLE	
			}

			free(AcceptedStr);
		}

		printf("Conversation ended.\n");
		closesocket(*t_socket);
		return result;
	}

	/*
		Description - Waiting for exit to close program.
		Parameters  - 
		Returns     - exit code.
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

		closeallClient();

		if (MainSocket != NULL)
		{
			closesocket(MainSocket);
			MainSocket = NULL;
		}

	}

	/*
		Description - Handle all the clients login
		Parameters  -
		Returns     - exit code.
	*/
	static DWORD HandleClients(void)
	{
		
		printf("Waiting for a client to connect...\n");
		// Setting the result.
		int result = NO_ERROR_VAL;

		// Waiting for  clients to connect.
		while (isToExit == FALSE_VAL)
		{
			SOCKET AcceptSocket = accept(MainSocket, NULL, NULL);

			// if accrepted.
			if (AcceptSocket == INVALID_SOCKET)
			{								
				closesocket(MainSocket);
				MainSocket = NULL;
				break;
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

		closeallClient();

		if (result < 0)
		{
			return(result);
		}
		else
		{
			WaitForMultipleObjects(NUM_OF_WORKER_THREADS, ThreadHandles, TRUE, INFINITE);

			printErrorFromClient();
		}
		
		return (result);
	}

#pragma endregion



