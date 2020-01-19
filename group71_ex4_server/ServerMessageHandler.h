//
//  ServerMessageHandler.h
//  Ex04_Server
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#ifndef ServerMessageHandler_h
#define ServerMessageHandler_h
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>


typedef struct SockParams
{
	SOCKET sd;
	int loc;
}SockParams;

#define BUFFER 10
#define SERVER_NAME "Server"
#define LEADER_FILE_LOC ".\\LeaderBoard.csv"
#define GAME_SESSION_LOC ".\\GameSession.txt"
#define IP_ADRESS_FILE ".\\out.txt"
#define TRUE 1
#define FALSE 0

#define PLAYER1_WIN "PLAYER 1"
#define PLAYER2_WIN "PLAYER 2"
#define DREW_IN_GAME "DRAW"
#define INVALID_MOVE_TXT "INVALID"
#define GAME_MOVE_INVALID 4
#define GAME_MOVE_VALID 5
#define NUM_OF_AVILABLE_NAMES 5
#define ROCK_OP 0
#define PAPER_OP 1
#define SCISOR_OP 2
#define LIZARD_OP 3
#define SPOCK_OP 4
#define MAX_CLIENTS 30
#define PLAYABLE_CLIENT 2
#define SERVER_DENIED_MESSAGE "To Manny Clients alrdy loged in."
#define SEND_ERROR_VAL 1200
#define FILE_ERROR 1300
#define MESSAGE_SIZE 1025
#define SCV_SPLIT ","
#define END_LINE "\n"
#define CVS_TEMPLATE "%s,%d,%d,%.3f\n"
#define MAX_LINE_CVS 30
#define SERVER_DENIE_CLIENT 124

char* getOtherMoveFromGameSessionFile(int* result);
int sendGeneralMesseage(char* messageID, SockParams * params);
int sendServerDenieMessage(char* messageID, char* message, SockParams * params);
int sendServerInvite(char* messageID, char name[], SockParams * params);
int sendGameResultMessage(char* messageID, char* client, char* moveOp, char* moveMe, char* won, SockParams * params);
int sendOponnentQuitMessage(char* messageID, char* otherClient, SockParams * params);
int sendLeaderBoardMessage(char* messageID, char* leaderFileContent, SockParams * params);
int pharseMessage(char* mssg, SockParams *param);
int pharseClientRequest(char* name, SockParams *param);
int pharseClientMainMenue(SockParams *param);
int pharseClientCPU(SockParams *param);
int pharseClientVS(SockParams *param);
int pharseClientLeader(SockParams *param,int isUpdate);
int pharseClientMove(char* move, SockParams *param);
int pharseClientReplay(SockParams *param);
int pharseClientRefresh(SockParams *param);
int pharseClientDisconnect(SockParams *param);
int handleWhowon(char OtherMove[], char* move, char* won, SockParams * param);
int isFileExist(const char * filename);
int isConnectedSever(SockParams * param);


#endif /* ServerMessageHandler_h */
