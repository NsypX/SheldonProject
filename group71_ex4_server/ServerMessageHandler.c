/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - client_main.
   Description -  this file is incharge of all the messages from the client
				  it create and send new messages, and it know how to pharse client 
				  messages.
*/

#pragma region Includes

	#define _CRT_SECURE_NO_WARNINGS
	#include <string.h>
	#include <stdlib.h>
	#include "ServerMessageHandler.h"
	#include "SocketSendRecvTools.h"
	#include "ServerHadnler.h"
	#include "Server.h"
	#include "LeaderBoarrd.h"
	#include "DefenitionsForServerClient.h"

#pragma endregion

#pragma region Globals

	SockParams* firstPlayer = NULL;
	SockParams* secondPlayer = NULL;

	char firstPlayerName[MAX_NAME];
	char secondPlayerName[MAX_NAME];
	int isVsPlayer[NUM_OF_AVILABLE_NAMES] = { FALSE_VAL,FALSE_VAL,FALSE_VAL,FALSE_VAL,FALSE_VAL };

#pragma endregion

#pragma region SendMessageFunctions


	/*
		Description- send general mssg to params.
		Parameters- messageID- the message id to be set.
					params - sock params (location and socket)
		Returns-    Error handle
	 */
	int sendGeneralMesseage(char* messageID, SockParams * params)
	{
		char* mssg = calloc(1, strlen(messageID) + BUFFER);

		if (mssg == NULL)
		{
			return (MALLOC_ERROR);
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

	/*
		Description- send server denie message
		Parameters- messageID- the message id to be set.
					message- to be printed on client.
					params - sock params (location and socket)
		Returns-    Error handle
	 */
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

	/*
		Description- send general mssg to params.
		Parameters- messageID- the message id to be set.
					name- the name of the client inviting us.
					params - sock params (location and socket)
		Returns-    Error handle
	 */
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

	/*
		Description- send game result.
		Parameters- messageID- the message id to be set.
					client- the client name.
					move op- move opponent.
					move me- my move.
					won- who won.
					params - sock params (location and socket)
		Returns-    Error handle
	 */
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

	/*
		Description- Send opponent quit.
		Parameters- messageID- the message id to be set.
					otherclient- the other client name.
					params - sock params (location and socket)
		Returns-    Error handle
	 */
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

	/*
		Description- send general mssg to params.
		Parameters- messageID- the message id to be set.
					leaderFileContent- txt of the leader file.
					params - sock params (location and socket)
		Returns-    Error handle
	 */
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

	/*
		Description- pharse client request.
		Parameters- name - the name of the client.
					params - sock params (location and socket)
		Returns-    Error handle
	*/
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

			if (result < 0)
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

	/*
		Description- pharse client main menue
		Parameters- params - sock params (location and socket)
		Returns-    Error handle
	*/
	int pharseClientMainMenue(SockParams * param)
	{
		int result = sendGeneralMesseage(SERVER_MAIN_MENU, param);

		return (NO_ERROR_VAL);
	}

	/*
		Description- pharse client cpu.
		Parameters-	params - sock params (location and socket)
		Returns-    Error handle
	*/
	int pharseClientCPU(SockParams * param)
	{
		int result = sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, param);
		isVsPlayer[param->loc] = FALSE_VAL;
		return(NO_ERROR_VAL);
	}

	/*
		Description- pharse client vs.
		Parameters-	params - sock params (location and socket)
		Returns-    Error handle
	*/
	int pharseClientVS(SockParams* param)
	{
		int result = NO_ERROR_VAL;
		
		result = waitGameSessionMutex();

		if (result < 0)
		{
			return(result);
		}

		if (isFileExist(GAME_SESSION_LOC) == FALSE_VAL)
		{
			firstPlayer = param;
		
			// Creating the file.
			result = createEmptyGameSession();

			if (result < 0)
			{
				return(result);
			}

			// Letting second player in.
			releaseGameSessionMutex();
		
			// Waiting for other player to arrive.
			result = waitOtherPlayerMove();

			// If arrived.
			if (result >= 0)
			{			
				// Set mutex to send messages.
				result = waitGameSessionMutex();

				if (result < 0)
				{
					return(result);
				}

				sendServerInvite(SERVER_INVITE, getName(secondPlayer->loc), firstPlayer);
				sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST,firstPlayer);
				releaseGameSessionMutex();
			}
			else
			{	
				// Else no opponent found.
				remove(GAME_SESSION_LOC);
				sendGeneralMesseage(SERVER_NO_OPPONENTS, param);
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
			result = waitGameSessionMutex();

			if (result < 0)
			{
				return(result);
			}

			sendServerInvite(SERVER_INVITE,getName(firstPlayer->loc), secondPlayer);
			sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, secondPlayer);
			isVsPlayer[param->loc] = TRUE_VAL;

			releaseGameSessionMutex();
		}

		return(NO_ERROR_VAL);
	}

	/*
		Description- pharse client leader.
		Parameters-	params - sock params (location and socket)
					isUpdate- is there anything to update in the file.
		Returns-    Error handle
	*/
	int pharseClientLeader(SockParams * param, int isUpdate)
	{
		int result = NO_ERROR_VAL;

		char* leaderFile = NULL;

		if (isUpdate == FALSE_VAL)
		{
			leaderFile = getLeaderInstanseFileFormat(&result);
		}
		else
		{
			leaderFile = getLeaderInstanseFileFormat(&result);
		}

		if (result < 0)
		{
			return(result);
		}

		if (leaderFile == NULL)
		{
			return (MALLOC_ERROR);
		}

		result = sendLeaderBoardMessage(SERVER_LEADERBOARD, leaderFile, param);

		free(leaderFile);

		// Create Leader Menue.
		result = sendGeneralMesseage(SERVER_LEADERBORAD_MENU, param);

		return(result);
	}

	/*
		Description- pharse client move.
		Parameters-	params - sock params (location and socket)
					move- the clients move.
		Returns-    Error handle
	*/
	int pharseClientMove(char* move, SockParams * param)
	{
		char OtherMove[LINE_SIZE] = "";

		int result = NO_ERROR_VAL;

		if ((isVsPlayer[param->loc] == TRUE_VAL) || (isVsPlayer[secondPlayer->loc] == TRUE_VAL))
		{
			// Check if we are the first or second player.
			if (param->loc == secondPlayer->loc)
			{
				// Wait for player 2 to write his move.
				waitGameSessionMutex();
			
				// Get player 2 move.
				strcpy(OtherMove,getOtherMoveFromGameSessionFile(result));

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
			else if (param->loc == firstPlayer->loc)
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
				strcpy(OtherMove, getOtherMoveFromGameSessionFile(result));

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

		// Send messages based.
		handleWhowon(OtherMove, move, won, param);


		// Result.
		result = sendGeneralMesseage(SERVER_GAME_OVER_MENU, param);

		return(result);
	}

	/*
		Description- pharse client replay.
		Parameters-	params - sock params (location and socket)
		Returns-    Error handle
	*/
	int pharseClientReplay(SockParams * param)
	{
		int result = NO_ERROR_VAL;

		// Check what kind of replay.
		if ((isVsPlayer[param->loc] == TRUE_VAL) || (isVsPlayer[secondPlayer->loc] == TRUE_VAL))
		{
			if (isFileExist(GAME_SESSION_LOC) == FALSE_VAL)
			{
				firstPlayer = param;

				// Creating the file.
				result = createEmptyGameSession();

				if (result < 0)
				{
					return(result);
				}

				// Letting second player in.
				releaseGameSessionMutex();

				// Waiting for other player to arrive.
				result = waitOtherPlayerMove();

				// If arrived.
				if (result >= 0)
				{
					sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, param);
				}
				else
				{
					// Else no opponent found.
					remove(GAME_SESSION_LOC);
					sendOponnentQuitMessage(SERVER_OPPONENT_QUIT, getName(secondPlayer->loc), param);
				}
			}
			else 
			{
				secondPlayer = param;

				// Delete file.
				remove(GAME_SESSION_LOC);

				// Releasing mutex.
				releaseGameSessionMutex();

				// Let other player move
				releaseOtherPlayerMove();

				// Ask for another moove.
				sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, param);	
			}
		}
		else
		{
			result = sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, param);
		}
	
		return(result);
	}

	/*
		Description- pharse client refresh.
		Parameters-	params - sock params (location and socket)
		Returns-    Error handle
	*/
	int pharseClientRefresh(SockParams * param)
	{
		int result = NO_ERROR_VAL;

		// Check update time.
		int isUpdate = getIsUpdated(&result);

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

	/*
		Description- pharse client disconnect.
		Parameters-	params - sock params (location and socket)
		Returns-    Error handle
	*/
	int pharseClientDisconnect(SockParams * param)
	{
		printf("Client number %d has disconnected.\n", param->loc);
		
		outCloseClient(param->loc);

		return(NO_ERROR_VAL);
	}

	/*
		Description- pharse client sheldon - a special suprise :).
		Parameters-	params - sock params (location and socket)
		Returns-    Error handle
	*/
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

	/*
		Description- pharse client message
		Parameters-	params - sock params (location and socket)
					mssg- mssg recived.
		Returns-    Error handle
	*/
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

#pragma region General Functions
	
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

	/*
		Description- create empty file.
		Parameters- 
		Returns-    Error handle
	*/
	int createEmptyGameSession()
	{
		FILE * gameSession = fopen(GAME_SESSION_LOC, "w");

		if (gameSession == NULL)
		{
			return(FILE_ERROR);
		}

		fclose(gameSession);
	}

	/*
		Description- Read the move from file.
		Parameters- result- error handler.
		Returns-    
	*/
	char* getOtherMoveFromGameSessionFile(int* result)
	{
		char OtherMove[20];

		FILE* sessionFile = fopen(GAME_SESSION_LOC, "r");

		if (sessionFile == NULL)
		{
			*result = FILE_READ_ERROR;
		}
		else
		{
			// Read the first line to pharse. (name/win/lose/ratio)
			fgets(OtherMove, LINE_SIZE, sessionFile);
			fclose(sessionFile);
		}

		return(OtherMove);
	}

	/*
		Description- Write the move to game session file.
		Parameters- move- the move to write to game session.
		Returns-    Error handle
	*/
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

	/*
		Description- Handle the winner.
		Parameters-	OtherMove
					move- my move.
					params- socket params.
		Returns-    Error handle
	*/
	int handleWhowon(char OtherMove[], char* move, char* won, SockParams * param)
	{
		int result = NO_ERROR_VAL;

		

		if (param->loc == firstPlayer->loc)
			{
				if (strcmp(won, PLAYER1_WIN) == 0)
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, getName(secondPlayer->loc), OtherMove, move, getName(firstPlayer->loc), param);

					addToLeaderInstanse(getName(firstPlayer->loc), 1, 0, &result);
					addToLeaderInstanse(getName(secondPlayer->loc), 0, 1, &result);

					if (result < 0)
					{
						return(result);
					}
				}
				else if (strcmp(won, PLAYER2_WIN) == 0)
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, getName(secondPlayer->loc), OtherMove, move, getName(secondPlayer->loc), param);

					addToLeaderInstanse(getName(secondPlayer->loc), 1, 0, &result);
					addToLeaderInstanse(getName(firstPlayer->loc), 0, 1, &result);
				}
				else
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, getName(secondPlayer->loc), OtherMove, move, DREW_IN_GAME, param);
				}
			}
			else if (param->loc == secondPlayer->loc)
			{
				if (strcmp(won, PLAYER1_WIN) == 0)
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, getName(firstPlayer->loc), OtherMove, move, getName(secondPlayer->loc), param);

					addToLeaderInstanse(getName(firstPlayer->loc), 1, 0, &result);
					addToLeaderInstanse(getName(secondPlayer->loc), 0, 1, &result);
				}
				else if (strcmp(won, PLAYER2_WIN) == 0)
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, getName(firstPlayer->loc), OtherMove, move, getName(firstPlayer->loc), param);

					addToLeaderInstanse(getName(secondPlayer->loc), 1, 0, &result);
					addToLeaderInstanse(getName(firstPlayer->loc), 0, 1, &result);
				}
				else
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, getName(firstPlayer->loc), OtherMove, move, DREW_IN_GAME, param);
				}
			}
		
			else
			{
				if (strcmp(won, PLAYER1_WIN) == 0)
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, OtherMove, move, getName(param->loc), param);

					addToLeaderInstanse(getName(param->loc), 1, 0, &result);
					addToLeaderInstanse(SERVER_NAME, 0, 1, &result);
				}
				else if (strcmp(won, PLAYER2_WIN) == 0)
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, OtherMove, move, SERVER_NAME, param);

					addToLeaderInstanse(SERVER_NAME, 1, 0, &result);
					addToLeaderInstanse(getName(param->loc), 0, 1, &result);
				}
				else
				{
					result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, OtherMove, move, DREW_IN_GAME, param);
				}
			}
	}

#pragma endregion
