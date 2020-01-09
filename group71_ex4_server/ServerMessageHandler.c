//
//  ServerMessageHandler.c
//  Ex04_Server
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#pragma region Includes

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include "ServerMessageHandler.h"
#include "SocketSendRecvTools.h"
#include "ServerHadnler.h"
#include "LeaderBoarrd.h"
#include "DefenitionsForServerClient.h"
#pragma endregion

#pragma region Globals

SockParams* firstPlayer = NULL;
SockParams* secondPlayer = NULL;

char firstPlayerName[MAX_NAME];
char secondPlayerName[MAX_NAME];
int isVsPlayer = FALSE_VAL;

#pragma endregion

#pragma region SendMessageFunctions

int sendGeneralMesseage(char* messageID, SockParams * params)
{
	char* mssg = calloc(1, strlen(messageID) + BUFFER);

	if (mssg == NULL)
	{
		return (NULL);
	}

	mssg = strcat(mssg, messageID);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, (params->sd));
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(params->sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}


int sendServerDenieMessage(char* messageID, char* message, SockParams * params)
{
	char* mssgDenie = calloc(1, strlen(messageID) + strlen(message) + BUFFER);

	if (mssgDenie == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssgDenie = strcat(mssgDenie, messageID);
	mssgDenie = strcat(mssgDenie, PARAMETER_BEGIN_CHAR);
	mssgDenie = strcat(mssgDenie, message);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssgDenie, (params->sd));
	free(mssgDenie);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket((params->sd));
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

int sendServerInvite(char* messageID, char name[], SockParams * params)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(name) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, name);

	// send mssg.	
	TransferResult_t SendRes = SendString(mssg, (params->sd));
	free(mssg);


	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket((params->sd));
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

int sendGameResultMessage(char* messageID, char* client, char* moveOp, char* moveMe, char* won, SockParams * params)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(client) + strlen(moveOp)
		+ strlen(moveMe) + strlen(won) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, client);
	mssg = strcat(mssg, PARAMETER_SPLIT_CHAR);
	mssg = strcat(mssg, moveOp);
	mssg = strcat(mssg, PARAMETER_SPLIT_CHAR);
	mssg = strcat(mssg, moveMe);
	mssg = strcat(mssg, PARAMETER_SPLIT_CHAR);
	mssg = strcat(mssg, won);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, (params->sd));
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket((params->sd));
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

int sendOponnentQuitMessage(char* messageID, char* otherClient, SockParams * params)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(otherClient) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, otherClient);
	

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, (params->sd));
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket((params->sd));
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

int sendLeaderBoardMessage(char* messageID, char* leaderFileContent, SockParams * params)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(leaderFileContent) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, leaderFileContent);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, (params->sd));
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket((params->sd));
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

#pragma endregion

#pragma region Pharseing Functions

int pharseClientRequest(char* name, SockParams * param)
{
	int result = NO_ERROR_VAL;
	if (isLocationAvilableForClient() == TRUE_VAL)
	{
		changeName(name,param->loc);
		result = sendGeneralMesseage(SERVER_APPROVED, param);
		result = sendGeneralMesseage(SERVER_MAIN_MENU, param);
		increaseCountLogged();
	}
	else
	{
		result = sendServerDenieMessage(SERVER_DENIED, SERVER_DENIED_MESSAGE, param);

		if (result != NO_ERROR_VAL)
		{
			return(result);
		}
		else
		{
			return(SERVER_DENIE_CLIENT);
		}
	}
	
	return (result);
}

int pharseClientMainMenue(SockParams * param)
{
	int result = sendGeneralMesseage(SERVER_MAIN_MENU, param);

	return (NO_ERROR_VAL);
}

int pharseClientCPU(SockParams * param)
{
	int result = sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, param);
	isVsPlayer = FALSE_VAL;
	return(NO_ERROR_VAL);
}

/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int isFileExist(const char * filename) 
{
	/* try to open file to read */
	FILE *file;

	if (file = fopen(filename, "r"))
	{
		fclose(file);
		return TRUE_VAL;
	}

	return FALSE_VAL;
}

void debugThread(void)
{
	while (1)
	{
		system("cls");
		printf("Waiting...");
	}
}

int createEmptyGameSession()
{
	FILE * gameSession = fopen(GAME_SESSION_LOC, "w");

	if (gameSession == NULL)
	{
		return(FILE_ERROR);
	}

	fclose(gameSession);
}

int pharseClientVS(SockParams* param)
{
	waitGameSessionMutex();

	if (isFileExist(GAME_SESSION_LOC) == FALSE_VAL)
	{
		firstPlayer = param;
		
		// Creating the file.
		createEmptyGameSession();

		// Letting second player in.
		releaseGameSessionMutex();
		
		// Waiting for other player to arrive.
		int waitTime = waitOtherPlayerMove();

		// If arrived.
		if (waitTime == 0)
		{			
			// Set mutex to send messages.
			waitGameSessionMutex();
			sendServerInvite(SERVER_INVITE, getName(secondPlayer->loc), firstPlayer);
			sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST,firstPlayer);
			releaseGameSessionMutex();
		}
		else
		{	
			// Else no opponent found.
			sendGeneralMesseage(SERVER_NO_OPPONENTS, param->sd);
		}
	}
	else
	{
		secondPlayer = param;

		// Releasing mutex.
		releaseGameSessionMutex();		

		// Let other player move
		releaseOtherPlayerMove();

		// Setting mutex to send messages.
		waitGameSessionMutex();

		sendServerInvite(SERVER_INVITE,getName(firstPlayer->loc), secondPlayer);
		sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, secondPlayer);
		isVsPlayer = TRUE_VAL;

		releaseGameSessionMutex();
	}

	return(NO_ERROR_VAL);
}

int pharseClientLeader(SockParams * param, int isUpdate)
{
	char* leaderFile = NULL;

	if (isUpdate == FALSE_VAL)
	{
		leaderFile = getLeaderInstanseFileFormat();
	}
	else
	{
		leaderFile = calloc(1,1);
	}

	if (leaderFile == NULL)
	{
		return (MALLOC_ERROR);
	}

	int result = sendLeaderBoardMessage(SERVER_LEADERBOARD, leaderFile, param);

	free(leaderFile);

	// Create Leader Menue.
	result = sendGeneralMesseage(SERVER_LEADERBORAD_MENU, param);

	return(NO_ERROR_VAL);
}

char* getOtherMoveFromGameSessionFile(int result)
{
	char OtherMove[20];

	FILE* sessionFile = fopen(GAME_SESSION_LOC, "r");

	if (sessionFile == NULL)
	{
		return(FILE_READ_ERROR);
	}
	else
	{
		// Read the first line to pharse. (name/win/lose/ratio)
		fgets(OtherMove, LINE_SIZE, sessionFile);
		fclose(sessionFile);
	}

	return(OtherMove);
}

int writeMoveToGameSession(char move[])
{
	FILE *sessionFile = fopen(GAME_SESSION_LOC, "w");

	if (sessionFile == NULL)
	{
		return(FILE_READ_ERROR);
	}
	else
	{
		fputs(move, sessionFile);
		fclose(sessionFile);
	}
}

int pharseClientMove(char* move, SockParams * param)
{
	char OtherMove[LINE_SIZE] = "";

	if (isVsPlayer == TRUE_VAL)
	{
		// Check if we are the first or second player.
		if (param->loc == firstPlayer->loc)
		{
			// Wait for player 2 to write his move.
			waitGameSessionMutex();
			
			// Get player 2 move.
			getOtherMoveFromGameSessionFile(OtherMove);

			// Write my move.
			writeMoveToGameSession(move);

			releaseGameSessionMutex();

			// Release second player.
			releaseOtherPlayerMove();

			// Let other player get my move.
			waitOtherPlayerMoveINF();

			// Read inside mutex
			waitGameSessionMutex();

			remove(GAME_SESSION_LOC);

			releaseGameSessionMutex();
		}
		else if (param->loc == secondPlayer->loc)
		{
			// Writing the move protected by mutex
			waitGameSessionMutex();
			
			// Write my move
			writeMoveToGameSession(move);

			releaseGameSessionMutex();

			// Wait for player one to write his move.
			waitOtherPlayerMoveINF();

			// Read inside mutex
			waitGameSessionMutex();

			// Get player 1 move.
			getOtherMoveFromGameSessionFile(OtherMove);

			releaseGameSessionMutex();

			// Release first player
			releaseOtherPlayerMove();
		}
	}
	else
	{	
		// Get cpu move.
		strcpy(OtherMove,getRandMove());		
	}

	// Check who won.
	char* won = checkWin(move, OtherMove);
	int result = NO_ERROR_VAL;

	if (strcmp(won, PLAYER1_WIN) == 0)
	{
		result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, OtherMove, move, getName(param->loc), param);

		addToLeaderInstanse(getName(param->loc), 1, 0);
		addToLeaderInstanse(SERVER_NAME, 0, 1);
	}
	else if (strcmp(won, PLAYER2_WIN) == 0)
	{
		result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, OtherMove, move, SERVER_NAME, param);

		addToLeaderInstanse(SERVER_NAME, 1, 0);
		addToLeaderInstanse(getName(param->loc), 0, 1);
	}
	else
	{
		result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, OtherMove, move, DREW_IN_GAME, param);
	}


	result = sendGeneralMesseage(SERVER_GAME_OVER_MENU, param);
	return(NO_ERROR_VAL);
}

int pharseClientReplay(SockParams * param)
{
	int result = sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, param);
	return(NO_ERROR_VAL);
}

int pharseClientRefresh(SockParams * param)
{
	// Check update time.
	int isUpdate = getIsUpdated();

	if (isUpdate == TRUE_VAL)
	{
		return(pharseClientLeader(param, FALSE_VAL));
		setUpdateTime();
	}
	else
	{
		return(pharseClientLeader(param, TRUE_VAL));
	}
}

int pharseClientDisconnect(SockParams * param)
{
	printf("\nWe SUCKKKKKKKKKKKKKKKKKKKKKKK AND DONT DO ANYTHING TO DISCONNECT YOU PROPERLY\n");

	return(NO_ERROR_VAL);
}

int pharseSheldon(SockParams * param)
{
	sendServerDenieMessage(SERVER_SHELDON, "Pick your move.", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "Pick your move..", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "Pick your move...", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "Pick your move....", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "!##!#!#!#!#!#!#!#!#!#!#! UGHHH TOO SLOW !##!#!#!#!#!#!#!#!#!", param);
	Sleep(1500);
	sendGeneralMesseage(SERVER_SPOCK, param);
	Sleep(3000);
	sendServerDenieMessage(SERVER_SHELDON, "I pick Spock, i Win!", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "I pick Spock, i Win!!", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "I pick Spock, i Win!!!", param);
	Sleep(500);
	sendServerDenieMessage(SERVER_SHELDON, "I pick Spock, i Win!!!!", param);
	Sleep(500);
	sendGeneralMesseage(SERVER_BAZINGA, param);
	Sleep(5000);
	sendGeneralMesseage(SERVER_MAIN_MENU, param);

	return(NO_ERROR_VAL);
}

int pharseMessage(char* mssg, SockParams * param)
{
	int result = NO_ERROR_VAL;

	if (mssg == NULL)
	{
		return(DISCONNECTED_FROM_SERVER);
	}

	char * header = strtok(mssg, PARAMETER_BEGIN_CHAR);
	char* params = strtok(NULL, PARAMETER_BEGIN_CHAR);

	if (strcmp(header, CLIENT_REQUEST) == 0)
	{
		result = pharseClientRequest(params, param);
	}
	else if (strcmp(header, CLIENT_MAIN_MENU) == 0)
	{
		result = pharseClientMainMenue(param);
	}
	else if (strcmp(header, CLIENT_CPU) == 0)
	{
		result = pharseClientCPU(param);
	}
	else if (strcmp(header, CLIENT_VERSUS) == 0)
	{
		result = pharseClientVS(param);
	}
	else if (strcmp(header, CLIENT_LEADERBOARD) == 0)
	{
		result = pharseClientLeader(param, FALSE_VAL);
	}
	else if (strcmp(header, CLIENT_PLAYER_MOVE) == 0)
	{
		result = pharseClientMove(params, param);
	}
	else if (strcmp(header, CLIENT_REPLAY) == 0)
	{
		result = pharseClientReplay(param);
	}
	else if (strcmp(header, CLIENT_REFRESH) == 0)
	{
		result = pharseClientRefresh(param);
	}
	else if (strcmp(header, CLIENT_DISCONNECT) == 0)
	{
		result = pharseClientDisconnect(param);
	}
	else if (strcmp(header, CLIENT_SHELDON) == 0)
	{
		result = pharseSheldon(param);
	}
	else
	{
		printf(UNSUPPURTED_MESSAGE);
	}
	return (result);
}

#pragma endregion