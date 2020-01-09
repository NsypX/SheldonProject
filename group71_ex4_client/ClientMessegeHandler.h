//
//  ClientMessegeHandler.h
//  Ex04_Client
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#ifndef ClientMessegeHandler_h
#define ClientMessegeHandler_h

#pragma region Includes

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "SocketSendRecvTools.h"
#include "Client.h"
#include "MessegeHead.h"

#pragma endregion

#pragma region Define Consts

	#pragma region Numbers

		#define SERVER_MENUE_OPT_AMOUNT 5
		#define SERVER_GAME_OVER_AMOUNT 2
		#define SERVER_LEADER_MENU_AMOUNT 2
		#define SERVER_DENIE_OPT_REFRESH 1
		#define SERVER_DENIE_OPT_EXIT 2
		#define SERVER_DENIE_RETRY 29

		#define RECONNECT_OPTION 1
		#define QUIT_OPTION 2
		#define MSSG_RECIVE_SIZE 1024

		#define SERVER_LEADER_REFRESH_OPT 1
		#define SERVER_LEADER_RETURN 2

		#define SCV_SPLIT ","
		#define END_LINE "\n"

		#define INPUT_TXT_SIZE 15
		#define SERVER_MENUE_OPTION_PLAY_VS_CLIENT 1
		#define SERVER_MENUE_OPTION_PLAY_VS_SERVER 2
		#define SERVER_MENUE_OPTION_VIEW_LEADER 3
		#define SERVER_MENUE_OPTION_QUIT 4
		#define SERVER_MENUE_OPTION_SHELDON 5

		#define SERVER_GAME_OVER_OPTION_ANOTHER_GAME 1
		#define SERVER_GAME_OVER_OPTION_RETURN 2
		#define SERVER_DENIE_ERROR -7
		#define BUFFER 15

	#pragma endregion

	#pragma region Strings

		#define SERVER_MENUE_MESSAGE  "Choose what to do next:\n1. Play against another client\n2. Play against the server\n3. View the leaderboard\n4. Quit\n5. Play Against Sheldon\n"
		#define SERVER_GAME_OVER_MESSAGE "\nChoose what to do next:\n1. Play again\n2. Return to the main menu\n"
		#define INPUT_ERROR_MESSAGE "Please enter a valid input.\n"
		#define SERVER_MOVE_REQUEST_MESSAGE "Choose a move from the list: Rock, Paper, Scissors, Lizard or Spock:\n"
		#define SERVER_GAME_RESULT_MESSAGE "You played: %s\n%s played: %s\n%s won!\n"
		#define SERVER_GAME_RESULT_MESSAGE_DREW "You played: %s\n%s played: %s\nThe game resulted in Draw!\n"
		#define SERVER_LEADER_MENU  "\nChoose what to do next:\n1. Refresh leaderboard\n2. Return to the main menu\n"


		#define SERVER_DENIED_MESSAGE "To many Clients already log in."
		#define SERVER_LEADER_UP_TO_DATE "The leaderboard you view is already up to date.\n"

	#pragma endregion

#pragma endregion

#pragma region Public Functions
	
	#pragma region pharseMessages

		int pharseMessage(char* mssg, SOCKET sd);

	#pragma endregion

	#pragma region InputFunctions

		
		void clearScrean(char* code);

	#pragma endregion

	#pragma region StringHandlers

			char* createTwoParramString(char* str, char* first, int second);
			char* createThreeParramString(char* str, char* first, char* second, int third);

	#pragma endregion

#pragma endregion

#endif /* ClientMessegeHandler_h */
