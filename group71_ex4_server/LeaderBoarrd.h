//
//  LeaderBoarrd.h
//  EX03_Hotel
//
//  Created by Dean Zahavy on 03/01/2020.
//  Copyright © 2020 Dean Zahavy. All rights reserved.
//

#ifndef LeaderBoarrd_h
#define _CRT_SECURE_NO_WARNINGS
#define LeaderBoarrd_h

#pragma region Defines

	#pragma region Structs

		typedef struct LeaderList
		{
			struct LeaderList * next;
			char * name;
			int win;
			int lost;
			float ratio;
		}LeaderList;

	#pragma endregion

	#pragma region Nums

		#define FILE_READ_ERROR -200
		#define NO_ERROR_VAL -300
		#define MAX_NAME 20
		#define MAX_LINE_LENGTH 100
		#define FALSE_VAL 0
		#define TRUE_VAL 1
		#define INF_VAL -1
		#define LINE_SIZE 30
		#define LINE_BUFFER 10
		#define MAX_FILE_SIZE 2000

	#pragma endregion

	#pragma region Strings

		#define LEADER_FILE_LOC ".\\LeaderBoard.csv"
		#define FILE_READ_MODE "r"
		#define SEPERATE_CHAR ","
		#define TEMPLATE "%s,%d,%d,%.3f\n"
		#define FILE_HEADER "Name,Won,Lost,Ratio(W/L)\n"

	#pragma endregion

#pragma endregion

#pragma region Functiosn

	#pragma region InstanseFunctions

	  LeaderList *bubbleSortLeaderBoard(LeaderList* lb);
		void freeLeaderInstanse(void);
		LeaderList *  getLeaderInstanse(void);
		void addToLeaderInstanse(char * name, int win, int lost);
		char * getLeaderInstanseFileFormat();
		LeaderList * getLeaderBoardFromFile(char * loc, LeaderList * lb, int * result);
		void freeLeader(LeaderList *currRoom);

	#pragma endregion

	#pragma region GeneralFunctions

		int getIsUpdated();

		int writeToFile(char* finalFileInput);

		void setUpdateTime(void);

		int LeaderBoardCreation();
	#pragma endregion

#pragma endregion

#endif /* LeaderBoarrd_h */
