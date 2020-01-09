//
//  ClientMessegeHandler.c
//  Ex04_Client
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#pragma region Includes
#define _CRT_SECURE_NO_WARNINGS
#include "ClientMessegeHandler.h"

	#pragma region PrivateFunctions

	#pragma region SendMessages

	int sendClientRequest(char* messageID, char* name, SOCKET sd);
	int sendClientMove(char* messageID, char* move, SOCKET sd);
	int sendGeneralMesseage(char* messageID, SOCKET sd);

	#pragma endregion

	#pragma region pharseMessages


	int pharseMainMenue(SOCKET sd);
	int pharseServerApproved(SOCKET sd);
	int pharseServerDenie(char* reason, SOCKET sd);
	int phasreServerGameResult(char* client, char* moveOp, char* moveMe, char* won);
	int pharseServerMoveRequest(SOCKET sd);
	int pharseGameOverMenue(SOCKET sd);
	int pharseServerLeader(char* board, SOCKET sd);
	int phraseServerLeaderMenue(SOCKET sd);
	int pharseNoOpponent(SOCKET sd);

	#pragma endregion

	#pragma region InputFunctions

	int getOptions(char * mssg, int optAmount);
	char* getName(char * mssg);
	char* getTXT(char * mssg);


	#pragma endregion

	#pragma endregion


#pragma endregion

#pragma region SendMessageFuncs
int sendClientRequest(char* messageID, char* name, SOCKET sd)
{
	// Calloc mssg size.
	char* mssg = calloc(1, strlen(messageID) + strlen(name) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	// Create mssg.
	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, name);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

int sendClientMove(char* messageID, char* move, SOCKET sd)
{
	// Calloc mssg size.
	char* mssg = calloc(1, strlen(messageID) + strlen(move) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	// Create mssg.
	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, move);

	// Send the mssg
	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}

int sendGeneralMesseage(char* messageID, SOCKET sd)
{
	// Calloc mssg size.
	char* mssg = calloc(1, strlen(messageID) + BUFFER);

	if (mssg == NULL)
	{
		return MALLOC_ERROR;
	}

	// Create mssg.
	mssg = strcat(mssg, messageID);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR_VAL);
}
#pragma endregion

#pragma region UserFucntions

void clearScrean(char* code)
{
	if ((strcmp(code, SERVER_GAME_OVER_MENU) != 0) &&
		(strcmp(code, SERVER_LEADERBORAD_MENU) != 0))
	{
		system("cls");
	}
}

int getOptions(char * mssg, int optAmount)
{
	// Set vars.
	char currMove = 'a';
	char optLastOption = optAmount + '0';
	int isFirstRun = 1;

	// Check if option chosen is avilable.
	while ((currMove > optLastOption) || (currMove < '1'))
	{
		// check if its the first run, or should print with errors.
		if (isFirstRun == 1)
		{
			isFirstRun++;
			printf("%s", mssg);
		}
		else
		{
			clearScrean("");
			printf("%s%s", mssg, INPUT_ERROR_MESSAGE);
		}

		// Get next cur
		currMove = getchar();

		// Check not end of std/ mouse click.
		if ((currMove == '\xff') || (currMove == '\251'))
		{
			return(INPUT_ERROR);
		}
	}

	// reading \n from std
	getchar();

	// Return the number chosen.
	return(currMove - '0');
}

char* getTXT(char * mssg)
{
	// Set vars.
	char currMove = 'a';
	char* result = calloc(1, INPUT_TXT_SIZE);

	// Check malloc error.
	if (result == NULL)
	{
		return(NULL);
	}

	// Set vars.
	short isValid = FALSE_VAL;
	short count = 0;

	// Continue while value isnt valid.
	while (isValid == FALSE_VAL)
	{
		// Save begin of string.
		char* helper = result;

		// Check if to print with error or not.
		if (count == 0)
		{
			count++;
			printf("%s", mssg);
		}
		else
		{
			clearScrean("");
			printf("%s%s", mssg, INPUT_ERROR_MESSAGE);
		}

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

			// if end of line
			if (currMove != '\n')
			{
				// Save the string in upper case.
				*helper = (char)toupper((int)currMove);
			}

			count++;
			helper++;
		}

		// set curr move = a.
		currMove = 'a';

		// set end of string.
		*helper = '\0';

		// make sure value is valid. if not recalloc it and try agin.
		if (result != NULL)
		{
			if ((strcmp(result, ROCK) == 0) ||
				(strcmp(result, PAPER) == 0) ||
				(strcmp(result, SCISSORS) == 0) ||
				(strcmp(result, LIZARD) == 0) ||
				(strcmp(result, SPOCK) == 0))
			{
				isValid = TRUE_VAL;
			}
			else
			{
				free(result);
				result = calloc(1, INPUT_TXT_SIZE);
			}
		}
		else
		{
			result = calloc(1, INPUT_TXT_SIZE);
		}

	}

	// return result.
	return(result);
}

char* getName(char * mssg)
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

	// Check if to print with error or not.
	printf("%s", mssg);

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
			*helper = currMove;
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
#pragma endregion

#pragma region Pharsing Functions

int phasreServerGameResult(char* client, char* moveOp, char* moveMe, char* won)
{
	// Printed formad.
	if (strcmp(won, DREW_IN_GAME) == 0)
	{
		printf(SERVER_GAME_RESULT_MESSAGE_DREW, moveMe, client, moveOp);
	}
	else
	{
		printf(SERVER_GAME_RESULT_MESSAGE, moveMe, client, moveOp, won);
	}

	return(CONTINUE_RUN);
}

int pharseMainMenue(SOCKET sd)
{
	// Set no error.
	int result = NO_ERROR_VAL;

	// Get main menue options.
	int option = getOptions(SERVER_MENUE_MESSAGE, SERVER_MENUE_OPT_AMOUNT);

	// Commit option.
	switch (option)
	{
	case (SERVER_MENUE_OPTION_PLAY_VS_CLIENT):
	{
		// Create cliend vs mssg.
		result = sendGeneralMesseage(CLIENT_VERSUS, sd);
		break;
	}
	case (SERVER_MENUE_OPTION_PLAY_VS_SERVER):
	{
		// Create client cpu mssg.
		result = sendGeneralMesseage(CLIENT_CPU, sd);
		break;
	}
	case (SERVER_MENUE_OPTION_VIEW_LEADER):
	{
		// Create client leaderboard mssg.
		result = sendGeneralMesseage(CLIENT_LEADERBOARD, sd);
		break;
	}
	case (SERVER_MENUE_OPTION_QUIT):
	{
		// Create client disconnect.
		result = sendGeneralMesseage(CLIENT_DISCONNECT, sd);
		return(TRUE_VAL);
		break;
	}
	case (SERVER_MENUE_OPTION_SHELDON):
	{
		// Create client disconnect.
		result = sendGeneralMesseage(CLIENT_SHELDON, sd);
		break;
	}
	default:
	{
		return(INPUT_ERROR);
		break;
	}
	}

	return(CONTINUE_RUN);
}

int pharseServerApproved(SOCKET sd)
{
	char* name = getClientName();

	if (name == NULL)
	{
		return(MALLOC_ERROR);
	}

	//int result = sendClientRequest(CLIENT_REQUEST, name, sd);
	return (CONTINUE_RUN);
}

int pharseSheldon(char* mssg, SOCKET sd)
{
	printf("%s", mssg);
	return (CONTINUE_RUN);
}

int pharseServerDenie(char* reason, SOCKET sd)
{
	// Close the socket to server.
	printf("IM CLOSING!!!");
	closeClient();
	return(DISCONNECT_TRY_CONNECT);
}

int pharseServerInvite(char* name)
{
	printf("The other client name is -> %s", name);
	return (CONTINUE_RUN);
}

int pharseServerMoveRequest(SOCKET sd)
{
	// Get next move.
	char* move = getTXT(SERVER_MOVE_REQUEST_MESSAGE);

	if (move == NULL)
	{
		return(INPUT_ERROR);
	}

	// Create move mssg.
	int result = sendClientMove(CLIENT_PLAYER_MOVE, move, sd);
	return (CONTINUE_RUN);
}

int phraseServerLeaderMenue(SOCKET sd)
{
	// Get game over options.
	int option = getOptions(SERVER_LEADER_MENU, SERVER_LEADER_MENU_AMOUNT);

	// Commit option.
	switch (option)
	{
		case (SERVER_LEADER_REFRESH_OPT):
		{
			// Create client replay.
			int result = sendGeneralMesseage(CLIENT_REFRESH, sd);
			break;
		}
		case (SERVER_LEADER_RETURN):
		{
			// Create main menue mssg
			int result = sendGeneralMesseage(CLIENT_MAIN_MENU, sd);
			break;
		}
		default:
		{
			return(INPUT_ERROR);
			break;
		}
	}

	return(CONTINUE_RUN);
}

int pharseServerLeader(char* board, SOCKET sd)
{
	if (board != NULL)
	{
		// Pharsing leaderboard file.
		char* line = strtok_s(board, END_LINE, &board);
		int counter = 0;

		while (line != NULL)
		{
			counter++;
			char* helper = _strdup(line);

			char* name = strtok(helper, SCV_SPLIT);
			char* won = strtok(NULL, SCV_SPLIT);
			char* lost = strtok(NULL, SCV_SPLIT);
			char* ratio = strtok(NULL, SCV_SPLIT);

			// print curr line values with 2 tabs, values padded 6 chars.
			printf(LEADER_BOARD_TEMPLATE, name, won, lost, ratio);

			// Get next line
			line = strtok_s(NULL, END_LINE, &board);
		}
	}
	else
	{
		printf(SERVER_LEADER_UP_TO_DATE);
	}

	return (CONTINUE_RUN);
}

int pharseGameOverMenue(SOCKET sd)
{
	// Get game over options.
	int option = getOptions(SERVER_GAME_OVER_MESSAGE, SERVER_GAME_OVER_AMOUNT);

	// Commit option.
	switch (option)
	{
	case (SERVER_GAME_OVER_OPTION_ANOTHER_GAME):
	{
		// Create client replay.
		int result = sendGeneralMesseage(CLIENT_REPLAY, sd);
		break;
	}
	case (SERVER_GAME_OVER_OPTION_RETURN):
	{
		// Create main menue mssg
		int result = sendGeneralMesseage(CLIENT_MAIN_MENU, sd);
		break;
	}
	default:
	{
		return(INPUT_ERROR);
		break;
	}
	}

	return(CONTINUE_RUN);
}

int pharseOpponentQuit(char* name)
{
	printf("%s\n", name);
	return (CONTINUE_RUN);
}

int pharseNoOpponent(SOCKET sd)
{
	printf("%s", SERVER_NO_OPPONENT_MSG);

	return(pharseMainMenue(sd));
	return(CONTINUE_RUN);
}

int pharseMessage(char* mssg, SOCKET sd)
{
	int result = CONTINUE_RUN;

	if (mssg == NULL)
	{
		return(DISCONNECT_TRY_CONNECT);
	}
	else if (strcmp(mssg, "") == 0)
	{
		return (DISCONNECT_TRY_CONNECT);
	}

	// Get messge id.
	char * header = strtok(mssg, PARAMETER_BEGIN_CHAR);

	// Get params (NULL if no params).
	char* params = strtok(NULL, PARAMETER_BEGIN_CHAR);

	clearScrean(header);

	// Check what mssg.
	if (strcmp(header, SERVER_MAIN_MENU) == 0)
	{
		return (pharseMainMenue(sd));
	}
	else if (strcmp(header, SERVER_APPROVED) == 0)
	{
		result = pharseServerApproved(sd);
	}
	else if (strcmp(header, SERVER_DENIED) == 0)
	{
		return(pharseServerDenie(params, sd));
	}
	else if (strcmp(header, SERVER_INVITE) == 0)
	{
		result = pharseServerInvite(params);
	}
	else if (strcmp(header, SERVER_PLAYER_MOVE_REQUEST) == 0)
	{
		result = pharseServerMoveRequest(sd);
	}
	else if (strcmp(header, SERVER_GAME_RESULTS) == 0)
	{
		char* client = strtok(params, PARAMETER_SPLIT_CHAR);
		char* moveOp = strtok(NULL, PARAMETER_SPLIT_CHAR);
		char* moveMe = strtok(NULL, PARAMETER_SPLIT_CHAR);
		char* won = strtok(NULL, PARAMETER_SPLIT_CHAR);
		result = phasreServerGameResult(client, moveOp, moveMe, won);
		return(result);
	}
	else if (strcmp(header, SERVER_GAME_OVER_MENU) == 0)
	{
		return(pharseGameOverMenue(sd));
	}
	else if (strcmp(header, SERVER_OPPONENT_QUIT) == 0)
	{
		result = pharseOpponentQuit(params);
	}
	else if (strcmp(header, SERVER_NO_OPPONENTS) == 0)
	{
		result = pharseNoOpponent(sd);
	}
	else if (strcmp(header, SERVER_LEADERBOARD) == 0)
	{
		result = pharseServerLeader(params, sd);
	}
	else if (strcmp(header, SERVER_LEADERBORAD_MENU) == 0)
	{
		result = phraseServerLeaderMenue(sd);
	}
	else if (strcmp(header, SERVER_SHELDON) == 0)
	{
		result = pharseSheldon(params, sd);
	}
	else if (strcmp(header, SERVER_BAZINGA) == 0)
	{
		printf("%s", SHELDON);
		result = CONTINUE_RUN;
	}
	else if (strcmp(header, SERVER_SPOCK) == 0)
	{
		printf("%s", SHELDON_SPOCK);
		result = CONTINUE_RUN;
	}
	else
	{
		printf(UNSUPPURTED_MESSAGE);
	}

	return (result);
}

#pragma endregion

#pragma region StringHandler
char* createTwoParramString(char* str, char* first, int second)
{
	size_t sz;

	// Getting the size needed to allocated
	sz = snprintf(NULL, 0, str, first, second);
	char * result = (char *)malloc(sz + 1);

	if (result != NULL)
	{
		// Getting the correct string.
		snprintf(result, sz + 1, str, first, second);
	}

	return(result);
}

char* createThreeParramString(char* str, char* first, char* second, int third)
{
	size_t sz;

	// Getting the size needed to allocated
	sz = snprintf(NULL, 0, str, first, second, third);
	char * result = (char *)malloc(sz + 1);

	if (result != NULL)
	{
		// getting the string.
		snprintf(result, sz + 1, str, first, second, third);
	}

	return(result);
}


#pragma endregion


