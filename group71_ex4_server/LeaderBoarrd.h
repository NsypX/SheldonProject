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

typedef struct LeaderList
{
    struct LeaderList * next;
    char * name;
    int win;
    int lost;
    float ratio;
}LeaderList;

#define LEADER_FILE_LOC ".\\LeaderBoard.csv"
#define FILE_READ_MODE "r"
#define SEPERATE_CHAR ","
#define FILE_READ_ERROR -200
#define NO_ERROR1 -300
#define MAX_NAME 20
#define MAX_LINE_LENGTH 100
#define FALSE_VAL 0
#define TRUE_VAL 1
#define INF_VAL -1
#define TEMPLATE "%s,%d,%d,%.3f\n"
#define FILE_HEADER "Name,Won,Lost,Ratio(W/L)\n"
#define LINE_SIZE 30
#define LINE_BUFFER 10
#define MAX_FILE_SIZE 2000



LeaderList * getLeaderBoardFromFile(char * loc, LeaderList * lb, int * result);

LeaderList * readNextLine(char * currLine, LeaderList * rl, int * result);

LeaderList *getNewLeaderLine(int* result);

LeaderList * addLineToList(char * name, int win, int lost, float ratio, LeaderList *lb, int * result);

void freeLeader(LeaderList *currRoom);

char* getCVSFormat(LeaderList* currLine);

char* getFullFileFormat(LeaderList * lb);

void freeLeaderInstanse(void);

LeaderList *  getLeaderInstanse(void);

void addToLeaderInstanse(char * name, int win, int lost);

char * getLeaderInstanseFileFormat();

LeaderList * removefromList(LeaderList * lb, char* name);

int getIsUpdated();

int writeToFile(char* finalFileInput);

void setUpdateTime(void);

int LeaderBoardCreation();


#endif /* LeaderBoarrd_h */
