//
//  ServerHadnler.h
//  Ex04_Server
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#ifndef ServerHadnler_h
#define ServerHadnler_h



#define LEADER_FILE_LOC ".\\LeaderBoard.csv"
#define TRUE 1
#define FALSE 0
#define PLAYER1_WIN "PLAYER 1"
#define PLAYER2_WIN "PLAYER 2"

#define INVALID_MOVE_TXT "INVALID"
#define GAME_MOVE_INVALID 4
#define GAME_MOVE_VALID 5
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

void changeName(char* name, int loc);
char* getRandMove(void);
char* checkWin(char* movePlayer1,char* movePlayer2);
int checkValidMove(char* move);
int random_number(int min_num, int max_num);
int writeToFile(char* name);
char* getName(int loc);
void cleanNamesList(void);

#endif /* ServerHadnler_h */
