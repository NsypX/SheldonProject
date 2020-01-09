/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
 This file was written for instruction purposes for the 
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering, Winter 2011, 
 by Amnon Drory.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef SOCKET_EXAMPLE_CLIENT_H
#define SOCKET_EXAMPLE_CLIENT_H

#pragma region Includes
	#include <stdio.h>
	#include <string.h>
	#include <winsock2.h>

	#include "MessegeHead.h"
	#include "SocketExampleShared.h"
	#include "SocketSendRecvTools.h"
	#include "ClientMessegeHandler.h"
#pragma endregion

#pragma region Define consts

	#pragma region Numbers
		#define TRUE_VAL 1
		#define FALSE_VAL 0
		#define MAX_NAME_SIZE 20
		#define PRINT_TIMEOUT 100
		#define eyal 100
		#define PRINT_DENIE 200
		#define PRINT_RETRY 300		
	#pragma endregion

	#pragma region Strings

		#define FAILED_CONNECTION_MSSG "Connection to server on %s:%d has been lost. Choose what to do next:\n1. Try to reconnect\n2. Exit\n"
		#define DISCONECT_MSSG "Connection to server on %s:%d has been lost. Choose what to do next:\n1. Try to reconnect\n2. Exit\n"
		#define SERVER_DENIE_MESSAGE_TEMP "Server Denied because- %s\nServer on %s:%d denied the connection request. Choose what to do next:\n1. Try to reconnect\n2. Exit\n"
		#define SERVER_CONNECTED_MESSAGE "Connected to server on %s:%d\n"

	#pragma endregion

#pragma endregion

#pragma region ClientPublicFunctions

	void clearScrean(char* code);
	void MainClient(char* ip, char* port, char* name);
	int closeClient();

	int setName(char* nameToSet);
	char* getClientName();
	char* getIP_ADRESS();
	int getPORT();

#pragma endregion

#endif // SOCKET_EXAMPLE_CLIENT_H