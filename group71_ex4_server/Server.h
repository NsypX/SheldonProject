/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
 This file was written for instruction purposes for the 
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering, Winter 2011, 
 by Amnon Drory.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef SOCKET_EXAMPLE_SERVER_H
#define SOCKET_EXAMPLE_SERVER_H
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include "SocketExampleShared.h"
#include "SocketSendRecvTools.h"
#include "ServerMessageHandler.h"
#include "DefenitionsForServerClient.h"
#include "LeaderBoarrd.h"
#include "ServerHadnler.h"

#define CLIENT_AMOUNT 2
#define WAIT_FOR_CLIENT_TIME 15000
#define INPUT_TXT_SIZE 100


void MainServer(char* ip);
static int FindFirstUnusedThreadSlot();
static void CleanupWorkerThreads();
static DWORD ServiceThread(SockParams *soc);
int isLocationAvilableForClient();
void increaseCountLogged(void);
int waitGameSessionMutex(void);
int releaseGameSessionMutex(void);

int waitOtherPlayerMove(void);
int releaseOtherPlayerMove(void);
int waitOtherPlayerMoveINF(void);
int waitFileMutex(void);
int releasFileMutex(void);




#endif // SOCKET_EXAMPLE_SERVER_H