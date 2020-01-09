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
#include "Client.h"

#pragma endregion

#pragma region GlobalVars

SOCKET m_socket;
HANDLE hThread;
int isDone = CONTINUE_RUN;
char ClientName[MAX_NAME_SIZE];

char IP_ADRESS[20];
int PORT;

#pragma endregion

#pragma region NameHandler
int setName(char* nameToSet)
{
	strcat(ClientName, nameToSet);
	return (NO_ERROR_VAL);
}

char* getClientName()
{
	return(ClientName);
}

char* getIP_ADRESS()
{
	return(IP_ADRESS);
}

int getPORT()
{
	return(PORT);
}
#pragma endregion


#pragma region Threads



int closeThread()
{
	TerminateThread(hThread, 0x555);
	CloseHandle(hThread);
	return(NO_ERROR_VAL);
}

//Reading data coming from the server
static DWORD RecvDataThread(void)
{
	TransferResult_t RecvRes;

	while (isDone == CONTINUE_RUN)
	{
		char *AcceptedStr = NULL;
		RecvRes = ReceiveString(&AcceptedStr, m_socket);

		if (RecvRes == TRNS_FAILED)
		{
			printf("Socket error while trying to write data to socket\n");
			return SERVER_TIMEOUT;
		}
		else if (RecvRes == TRNS_DISCONNECTED)
		{
			printf("Server closed connection. Bye!\n");
			return SERVER_TIMEOUT;
		}
		else
		{
			isDone = pharseMessage(AcceptedStr, m_socket);
		}

		free(AcceptedStr);
	}

	return isDone;
}


#pragma endregion

#pragma region NormalFunctions

int closeClient()
{
	closesocket(m_socket);

	WSACleanup();

	return TRUE_VAL;
}

void MainClient(char* ip, char* charPort, char* name)
{

	if (strlen(name) > MAX_NAME_SIZE)
	{
		printf("Name isnt valid. going down.");
		return;
	}
	SOCKADDR_IN clientService;

	// Initialize Winsock.
	WSADATA wsaData; //Create a WSADATA object called wsaData.

	int printOption = PRINT_RETRY;
	int connectHelper = SOCKET_ERROR;
	int opt = RECONNECT_OPTION;
	int tryToConnect = TRUE_VAL;

	strcpy(IP_ADRESS, ip);
	PORT = atoi(charPort);
	setName(name);


	while (opt != QUIT_OPTION)
	{
		// Set quit if not answered..
		opt = QUIT_OPTION;

		// Call WSAStartup and check for errors.
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
			printf("Error at WSAStartup()\n");

		// Create a socket.
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		// Check for errors to ensure that the socket is a valid socket.
		if (m_socket == INVALID_SOCKET) 
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			WSACleanup();
		}

		//Create a sockaddr_in object clientService and set  values.
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr(IP_ADRESS);
		clientService.sin_port = htons(PORT); 


		// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
		// Check for general errors.
		if (tryToConnect == TRUE_VAL)
		{
			tryToConnect = FALSE_VAL;
			connectHelper = connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService));
		}

		if (connectHelper == SOCKET_ERROR)
		{
			printf("Failed to connect.\n");
			WSACleanup();
			char* printed = NULL;

			switch (opt)
			{
				case(RECONNECT_OPTION):
				{
					printed = createTwoParramString(FAILED_CONNECTION_MSSG, getIP_ADRESS(), getPORT());
					break;
				}
				case(QUIT_OPTION):
				{
					printed = createTwoParramString(DISCONECT_MSSG, getIP_ADRESS(), getPORT());
					break;
				}
				case(PRINT_DENIE):
				{
					printed = createThreeParramString(SERVER_DENIE_MESSAGE_TEMP, SERVER_DENIED_MESSAGE, getIP_ADRESS(), getPORT());
					break;
				}
				default:
				{				
					break;
				}
			}
		

			if (printed == NULL)
			{
				return;
			}

			// try connect to server
			opt = getOptions(printed, 2);
			tryToConnect = TRUE_VAL;
			free(printed);
		}
		else
		{
			// Sending the name to the server.
			sendClientRequest(CLIENT_REQUEST, getClientName(), m_socket);

			// Run thread to read data from client.			
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvDataThread, NULL, 0, NULL);

			// Wait for threads
			WaitForSingleObject(hThread, INFINITE);

			// Get Exit Code 
			int exitcode = NO_ERROR;
			GetExitCodeThread(hThread, &exitcode);

			// Close Thread stuff.
			closeThread();

			// Close all
			closeClient();

			if (exitcode == DISCONNECT_TRY_CONNECT)
			{			
				opt = RECONNECT_OPTION;
				printOption = PRINT_DENIE;
				connectHelper = SOCKET_ERROR;
			}
			else if (exitcode == SERVER_TIMEOUT)
			{
				opt = RECONNECT_OPTION;
				printOption = PRINT_TIMEOUT;
				connectHelper = SOCKET_ERROR;
			}
		}
	}
	return;
}


#pragma endregion
