//
//  ServerHadnler.c
//  Ex04_Server
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//


#pragma region Includes
#define _CRT_SECURE_NO_WARNINGS
#include "ServerHadnler.h"
#include <string.h> //strlen
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include "MessegeHead.h"
#include "ServerMessageHandler.h"
#pragma endregion

#pragma region Globals
char nameList[NUM_OF_AVILABLE_NAMES][20];
#pragma endregion

#pragma region Unused



#pragma endregion

#pragma region NameHandleFunctions

void cleanNamesList(void)
{
	for (int i = 0; i < NUM_OF_AVILABLE_NAMES; i++)
	{
		strcpy(nameList[i], "");
	}
}

char* getName(int loc)
{
	return(nameList[loc]);
}

void changeName(char* name,int loc)
{
	if (strcmp(nameList[loc], "") == 0)
	{		
		strcat(nameList[loc], name);
	}
	else
	{
		strcpy(nameList[loc], name);
	}
}

#pragma endregion

#pragma region GameFunctions


int random_number(int min_num, int max_num)
{
	// Randomize int, takend from
	int result = 0, low_num = 0, hi_num = 0;

	if (min_num < max_num)
	{
		low_num = min_num;
		hi_num = max_num + 1; // include max_num in output
	}
	else
	{
		low_num = max_num + 1; // include max_num in output
		hi_num = min_num;
	}

	srand(time(NULL));
	result = (rand() % (hi_num - low_num)) + low_num;
	return result;
}

char* getRandMove(void)
{
	// rnadomize option from 0-4
	int opt = random_number(0, 4);

	// Get computer option.
	switch (opt)
	{
	case(ROCK_OP):
	{
		return(ROCK);
	}
	case(SCISOR_OP):
	{
		return(SCISSORS);
	}
	case(LIZARD_OP):
	{
		return(LIZARD);
	}
	case(SPOCK_OP):
	{
		return(SPOCK);
	}
	case(PAPER_OP):
	{
		return(PAPER);
	}
	default:
	{
		return(PAPER);
	}
	}
}

int checkValidMove(char* move)
{
	// Check if move is valid one.
	if ((strcmp(move, ROCK) == 0) ||
		(strcmp(move, PAPER) == 0) ||
		(strcmp(move, SCISSORS) == 0) ||
		(strcmp(move, LIZARD) == 0) ||
		(strcmp(move, SPOCK) == 0))
	{
		return (GAME_MOVE_VALID);
	}

	return(GAME_MOVE_INVALID);
}

char*  checkWin(char* movePlayer1, char* movePlayer2)
{
	// Check if a valid move
	if (checkValidMove(movePlayer1) == GAME_MOVE_INVALID)
	{
		return(INVALID_MOVE_TXT);
	}

	// Check if drew.
	if (strcmp(movePlayer1, movePlayer2) == 0)
	{
		return(DREW_IN_GAME);
	}
	// Check what move player 1 did, and compare to player 2.
	else if (strcmp(movePlayer1, ROCK) == 0)
	{
		if (strcmp(movePlayer2, SCISSORS) == 0)
		{
			return(PLAYER1_WIN);
		}
		else if (strcmp(movePlayer2, LIZARD) == 0)
		{
			return(PLAYER1_WIN);
		}
	}
	else if (strcmp(movePlayer1, PAPER) == 0)
	{
		if (strcmp(movePlayer2, SPOCK) == 0)
		{
			return(PLAYER1_WIN);
		}
		else if (strcmp(movePlayer2, ROCK) == 0)
		{
			return(PLAYER1_WIN);
		}
	}
	else if (strcmp(movePlayer1, SCISSORS) == 0)
	{
		if (strcmp(movePlayer2, PAPER) == 0)
		{
			return(PLAYER1_WIN);
		}
		else if (strcmp(movePlayer2, LIZARD) == 0)
		{
			return(PLAYER1_WIN);
		}
	}
	else if (strcmp(movePlayer1, LIZARD) == 0)
	{
		if (strcmp(movePlayer2, PAPER) == 0)
		{
			return(PLAYER1_WIN);
		}
		else if (strcmp(movePlayer2, SPOCK) == 0)
		{
			return(PLAYER1_WIN);
		}
	}
	else if (strcmp(movePlayer1, SPOCK) == 0)
	{
		if (strcmp(movePlayer2, SCISSORS) == 0)
		{
			return(PLAYER1_WIN);
		}
		else if (strcmp(movePlayer2, ROCK) == 0)
		{
			return(PLAYER1_WIN);
		}
	}
	else
	{
		return(INVALID_MOVE_TXT);
	}

	return(PLAYER2_WIN);
}

#pragma endregion

