/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - Client.
   Description - this module is incharge of the communication with the 
				 the server of this program.
				 base on resriction 8 from class.
*/

#pragma region Includes

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"

#pragma endregion

#pragma region GlobalVars
	
	// Socket
	SOCKET m_socket;

	// Thread handler.
	HANDLE hThread;

	// integer connecting the result of thread and the run of the process.
	int isDone = CONTINUE_RUN;

	// Client name
	char ClientName[MAX_NAME_SIZE];

	// Client ip
	char IP_ADRESS[20];

	// Client port.
	int PORT;

#pragma endregion

#pragma region NameHandler

	/*
		Description - save the name recived by the client.
		Parameters  - nameToSet
		Returns     - Error handle
	 */
	int setName(char* nameToSet)
	{
		strcat(ClientName, nameToSet);
		return (NO_ERROR_VAL);
	}

	/*
		Description - get the name of the client.
		Parameters  - 
		Returns     - name of client.
	 */
	char* getClientName()
	{
		return(ClientName);
	}

	/*
		Description - get the ip adress of the client.
		Parameters  - 
		Returns     - ip adress
	 */
	char* getIP_ADRESS()
	{
		return(IP_ADRESS);
	}

	/*
		Description - get the port of the client.
		Parameters  - 
		Returns     - port
	 */
	int getPORT()
	{
		return(PORT);
	}

#pragma endregion

#pragma region Threads

	/*
		Description - close thread and return error if exist.
		Parameters  - 
		Returns     - Error handle
		*/
	int closeThread()
	{
		TerminateThread(hThread, 0x555);
		CloseHandle(hThread);
		hThread = NULL;
		return(NO_ERROR_VAL);
	}

	/*
	Description - a thread getting data from the client.
	Parameters  - 
	Returns     - 
	*/
	static DWORD RecvDataThread(void)
	{
		TransferResult_t RecvRes;

		isDone = CONTINUE_RUN;

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

	/*
	Description - Closing the seocket of client and clean wsa.
	Parameters  - 
	Returns     - Error handle
	*/
	int closeClient()
	{
		closesocket(m_socket);
		return TRUE_VAL;
	}

	/*
	Description - Ask the client if to reconnect.
	Parameters  - tryToConnect- should i try or not.
				  opt- option to return.
	Returns     - Error handle
	*/
	int reConnectMenue(int* tryToConnect, int* opt)
	{
		int result = NO_ERROR_VAL;
		char* printed = NULL;

		// Get the print base on message needed.
		switch (*opt)
		{
			case(RECONNECT_OPTION):
			{
				printed = createTwoParramString(FAILED_CONNECTION_MSSG, getIP_ADRESS(), getPORT(),&result);
				break;
			}
			case(PRINT_TIMEOUT):
			{
				printed = createTwoParramString(DISCONECT_MSSG, getIP_ADRESS(), getPORT(), &result);
				break;
			}
			case(PRINT_DENIE):
			{
				printed = createThreeParramString(SERVER_DENIE_MESSAGE_TEMP, SERVER_DENIED_MESSAGE, getIP_ADRESS(), getPORT(), &result);
				break;
			}
			default:
			{
				break;
			}
		}

		// If malloc error
		if (printed == NULL)
		{
			return(MALLOC_ERROR);
		}

		// Ask the user for input.
		*opt = getOptions(printed, 2);
		*tryToConnect = TRUE_VAL;
		free(printed);

		return(result);
	}

	/*
	Description - Load client thread, connect to server and wait for thread to finish.
	Parameters  - ip- ip adress to connect.
				  port- the port to connect.
				  name- client name.
	Returns     - Error handle
	*/
	void MainClient(char* ip, char* charPort, char* name)
	{						
		// Initialize Winsock.
		WSADATA wsaData; int printOption = RECONNECT_OPTION; int connectHelper = SOCKET_ERROR; int opt = RECONNECT_OPTION; int tryToConnect = TRUE_VAL; int result = NO_ERROR_VAL; SOCKADDR_IN clientService;
		// Put 127.0.0.1 port and name
		strcpy(IP_ADRESS, ip);
		PORT = atoi(charPort);
		setName(name);		

		// While not quit.
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
			// Try to connect..
			if (tryToConnect == TRUE_VAL)
			{
				tryToConnect = FALSE_VAL;
				connectHelper = connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService));
			}
			// If connection failed.
			if (connectHelper == SOCKET_ERROR)
			{
				WSACleanup();
				reConnectMenue(&tryToConnect, &printOption);
				opt = printOption;
			}
			else
			{
				// Print connection message to client.
				char* printed = NULL;
				printed = createTwoParramString(SERVER_CONNECTED_MESSAGE, getIP_ADRESS(), getPORT(), &result);
				printf("%s", printed);
				free(printed);
				// Sending the name to the server.
				result = sendClientRequest(CLIENT_REQUEST, getClientName(), m_socket);
				// Run thread to read data from client.			
				hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvDataThread, NULL, 0, NULL);
				// Wait for threads
				result = WaitForSingleObject(hThread, INFINITE);
				if (result != WAIT_OBJECT_0)
				{
					result = MUTEX_ERROR;
				}

				// Get Exit Code 
				int exitcode = NO_ERROR;
				GetExitCodeThread(hThread, &exitcode);

				// Close Thread stuff.
				closeThread();
				// Close all
				closeClient();
				// Clena up
				WSACleanup();

				// Check exit code of client thread to know if message disconnect.
				if (exitcode >= 0)
				{
					if (exitcode == DISCONNECT_TRY_CONNECT)
					{			
						// Set params for relogin
						opt = RECONNECT_OPTION;
						printOption = PRINT_DENIE;
						connectHelper = SOCKET_ERROR;
					}
					else if (exitcode == SERVER_TIMEOUT)
					{
						// Set params for relogin
						opt = RECONNECT_OPTION;
						printOption = PRINT_TIMEOUT;
						connectHelper = SOCKET_ERROR;
					}
				}
				else
				{
					errorPrinter(exitcode);
				}
			}
			system("cls");
		}
}

#pragma endregion
