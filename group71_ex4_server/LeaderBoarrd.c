/*
   Authors     - Dean Zahavy 200770105, Eyal Seifert 205926025
   Project     - LeaderBoard
   Description - this module is incharge of handeling the leader board.
*/

#pragma region Includes
#include "LeaderBoarrd.h"
#include "DefenitionsForServerClient.h"
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#pragma endregion

#pragma region PrivateFunctions

	LeaderList * readNextLine(char * currLine, LeaderList * rl, int * result);

	LeaderList *getNewLeaderLine(int* result);

	LeaderList * addLineToList(char * name, int win, int lost, float ratio, LeaderList *lb, int * result);

	char* getCVSFormat(LeaderList* currLine);

	char* getFullFileFormat(LeaderList * lb);

	LeaderList * removefromList(LeaderList * lb, char* name);

#pragma endregion

#pragma region Globals

	LeaderList * currList = NULL;
	long updateTime= TRUE_VAL;

#pragma endregion

#pragma region InstanseFunctions

	/*
		Description - if no list was created, create the list from the file.
		Parameters  - 
		Returns     - LeaderList from the file.
		*/
	LeaderList *  getLeaderInstanse(void)
	{
		waitFileMutex();
		if (currList == NULL)
		{
			int result = NO_ERROR_VAL;
		
			result = LeaderBoardCreation();

			if (result != TRUE_VAL)
			{
				return(result);
			}

			currList = getLeaderBoardFromFile(LEADER_FILE_LOC, currList, &result);
		}
		releasFileMutex();
		return (currList);
	}

	/*
		free the list of the instanse.
	*/
	void freeLeaderInstanse(void)
	{
		waitFileMutex();
		freeLeader(currList);
		releasFileMutex();
	}

	/*
	Description - return a bool if the file is updated.
	Parameters  - 
	Returns     - TRUE/ FALSE
	*/
	int getIsUpdated()
	{
		waitFileMutex();
		if (updateTime < (long)time(NULL))
		{
			return(FALSE_VAL);
		}
		else
		{
			return(TRUE_VAL);
		}
		releasFileMutex();
	}

	/*
	Description - Setting the last update time by the user.
	Parameters  - 
	Returns     - 
	*/
	void setUpdateTime(void)
	{
		waitFileMutex();

		updateTime = (long)time(NULL);
		releasFileMutex();
	}

	/*
		 Description- add on the name givven the win/lost to the number at the table.
					  if name dont exist, create new instanse.
		 Parameters-	name- The name of the user.
						win- the amount of wins to add..
						lost- the amount of lost to add..
		 Returns-		nothing.
	*/
	void addToLeaderInstanse(char * name, int win, int lost)
	{
		waitFileMutex();

		int* result = -1;
		setUpdateTime();
		currList = addLineToList(name, win, lost, INF_VAL, currList, result);
		char* fileToSave = getFullFileFormat(currList);
		writeToFile(fileToSave);
		releasFileMutex();
	}

	/*
		get the leader board from instanse.
	*/
	char* getLeaderInstanseFileFormat()
	{
		waitFileMutex();
		return(getFullFileFormat(currList));
		releasFileMutex();
	}

#pragma endregion

#pragma region ListFunctions

	/*
	   Description- Create a list of leader board from a givven location
	   Parameters-  loc- the location of the room file.
					lb- room list.
					result- error handler.
	   Returns-     Room list, NULL if error.
	*/
	LeaderList * getLeaderBoardFromFile(char * loc, LeaderList * lb, int * result)
	{
		// Open id file for reads.
		FILE * currentFile = fopen(loc, FILE_READ_MODE);

		// Check if file was created.
		if (currentFile != NULL)
		{
			char nextLine[LINE_SIZE + LINE_BUFFER] = "";
			int len = 0;

			// Read the first line to pharse. (name/win/lose/ratio)
			fgets(nextLine, LINE_SIZE + LINE_BUFFER, currentFile);

			if (nextLine != NULL)
			{
				while (fgets(nextLine, LINE_SIZE + LINE_BUFFER, currentFile))
				{
					lb = readNextLine(nextLine, lb, result);
				}
			}
			else
			{
				fclose(currentFile);
				*result = MALLOC_ERROR;
			}

			// Closing the file.
			fclose(currentFile);
		}
		else
		{
			*result = FILE_READ_ERROR;
		}

		// Return result.
		return lb;
	}

	/*
	   Description- pharsing the next line of leaders and
					adding to list.
	   Parameters-  currLine- param.
					lb- Leader List
					result- error handler.
	   Returns-     Leader list, NULL if error.
	*/
	LeaderList * readNextLine(char * currLine, LeaderList * lb, int * result)
	{
		char * name = _strdup(currLine);

		if (name == NULL)
		{
			*result = MALLOC_ERROR;
		}
		else
		{
			name = strtok(name, SEPERATE_CHAR);
			int win = atoi(strtok(NULL, SEPERATE_CHAR));
			int lost = atoi(strtok(NULL, SEPERATE_CHAR));
			float ratio = (float)win;

			if (lost != 0)
			{
				ratio = (float)win / (float)lost;
			}

			lb = addLineToList(name, win, lost, ratio, lb, result);

			free(name);
		}

		return (lb);
	}

	/*
	   Description- add params to leader list.
	   Parameters-  name- person name
					win- win amount
					lost- lost amount
					ratio- ratio win/lost
					lb- leader list.
					result - error handler.
	   Returns-     Room list, NULL if error.
	*/
	LeaderList * addLineToList(char * name, int win, int lost, float ratio, LeaderList *lb, int * result)
	{
		// Creating the new line to add.
		LeaderList * lineToAdd = getNewLeaderLine(result);

		if (ratio == INF_VAL)
		{
			if (lost == 0)
			{
				ratio = (float)win;
			}
			else
			{
				ratio = (float)win / (float)lost;
			}
		}

		if (result < 0)
		{
			return(NULL);
		}

		lineToAdd->win = win;
		lineToAdd->lost = lost;

		// Check if lost == 0.
		if (lineToAdd->lost != 0)
		{
			lineToAdd->ratio = (float)lineToAdd->win / (float)lineToAdd->lost;
		}
		else
		{
			lineToAdd->ratio = (float)win;
		}

		lineToAdd->next = NULL;
		strcpy(lineToAdd->name, name);

		if (lb == NULL)
		{
			lb = lineToAdd;
		}
		else
		{
			LeaderList * currLine = lb;
			short isLast = TRUE_VAL;
			short isUpdated = FALSE_VAL;
			LeaderList * father = NULL;

			// Going till find the position to add sorted.
			while (currLine != NULL)
			{
				if (strcmp(currLine->name, name) == 0)
				{
					lineToAdd->win += currLine->win;
					lineToAdd->lost += currLine->lost;

					if (lineToAdd->lost != 0)
					{
						lineToAdd->ratio = (float)lineToAdd->win / (float)lineToAdd->lost;
					}
					else
					{
						lineToAdd->ratio = (float)win;
					}

					// Removing from list to add in order.
					lb = removefromList(lb, lineToAdd->name);
					lb = addLineToList(lineToAdd->name, lineToAdd->win, lineToAdd->lost, lineToAdd->ratio, lb, result);

					isUpdated = TRUE_VAL;
					break;
				}
			
				if (ratio > currLine->ratio)
				{
					isLast = FALSE_VAL;
					break;
				}

				father = currLine;
				currLine = currLine->next;
			}

			// Check if updated.
			if (isUpdated == FALSE_VAL)
			{
				// Check if last.
				if (isLast == FALSE_VAL)
				{
					// If to top of list.
					if (father == NULL)
					{
						lineToAdd->next = currLine;
						lb = lineToAdd;
					}
					else
					{
						lineToAdd->next = currLine;
						father->next = lineToAdd;
					}
				}
				else
				{
					father->next = lineToAdd;
				}
			}
		}

		return (lb);
	}

	/*
	   Description- Create a instance of new room
	   Parameters- result- error handler.
	   Returns-     Room list, NULL if error.
	*/
	LeaderList *getNewLeaderLine(int* result)
	{
		LeaderList *newLine = (LeaderList*)calloc(1, sizeof(LeaderList));

		if (newLine == NULL)
		{
			*result = MALLOC_ERROR;
		}

		newLine->name = calloc(1, MAX_NAME);
		newLine->next = NULL;

		if (newLine->name == NULL)
		{
			free(newLine);
			*result = MALLOC_ERROR;
			return(NULL);
		}

		return(newLine);
	}

	/*
	   Description- Create a list of guests from a givven location
	   Parameters-  loc- the location of the guest file.
					rl- room list.
					result- error handler.
	   Returns-     Room list, NULL if error.
	*/
	void freeLeader(LeaderList *rl)
	{
		LeaderList* currRoom = rl;

		while (currRoom->next != NULL)
		{
			LeaderList* helper = currRoom->next;
			free(currRoom->name);
			free(currRoom);

			currRoom = helper;
		}

		free(currRoom->name);
		free(currRoom);
	}

	/*
		remove at name
	*/
	LeaderList * removefromList(LeaderList * lb, char* name)
	{
		LeaderList * currLine = lb;
		LeaderList * father = NULL;

		while (currLine != NULL)
		{
			if (strcmp(currLine->name, name) == 0)
			{
				if (father == NULL)
				{
					lb = currLine->next;
				}
				else
				{
					father->next = currLine->next;
				}

				free(currLine->name);
				free(currLine);
				currLine = NULL;

				break;
			}

			father = currLine;
			currLine = currLine->next;
		}

		return(lb);
	}

#pragma endregion

#pragma region FileFunctions

	/*
	   Description- create a string by cvs format.
	   Parameters- currLine- curr cvs line to string format.
	   Returns-     line of cvs, NULL if error.
	*/
	char* getCVSFormat(LeaderList* currLine)
	{
		size_t sz;

		// Getting the size needed to allocated
		sz = snprintf(NULL, 0, TEMPLATE, currLine->name, currLine->win, currLine->lost, currLine->ratio);
		char * result = (char *)calloc(1, sz + 1);

		if (result != NULL)
		{
			// Getting the correct string.
			snprintf(result, sz + 1, TEMPLATE, currLine->name, currLine->win, currLine->lost, currLine->ratio);
		}

		return(result);
	}

	/*
   Description- get the list in a cvs format.
   Parameters- lb- the list to create
   Returns-     NUll if error, file else.
	*/
	char* getFullFileFormat(LeaderList * lb)
	{
		char* file = (char*)calloc(1, MAX_FILE_SIZE);
		file = strcat(file, FILE_HEADER);

		LeaderList* currLine = lb;

		while (currLine != NULL)
		{
			char* lineToAdd = getCVSFormat(currLine);
			file = strcat(file, lineToAdd);
			free(lineToAdd);
			currLine = currLine->next;
		}

		return(file);
	}

	/*
   Description- write string to file.
   Parameters-finalFileInput- the cvs file.
   Returns-     Error handle.
	*/
	int writeToFile(char* finalFileInput)
	{

		FILE* fp = fopen(LEADER_FILE_LOC, "w");
		fprintf(fp, "%s", finalFileInput);
		fclose(fp);

		return(NO_ERROR_VAL);
	}

	/*
   Description- Check if file exist.
   Parameters-
   Returns-   true/false
	*/
	int isLeaderBoardExist()
	{

		/* try to open file to read */
		FILE *file;

		if (file = fopen(LEADER_FILE_LOC, "r"))
		{
			fclose(file);
			return TRUE_VAL;
		}

		return FALSE_VAL;
	}

	/*
   Description- Create original leader board
   Parameters-
   Returns-     Error handle.
	*/
	int LeaderBoardCreation()
	{
		if (isLeaderBoardExist() != TRUE_VAL)
		{
			FILE* leader = fopen(LEADER_FILE_LOC, "w");

			if (leader == NULL)
			{
				return(FILE_READ_ERROR);
			}

			fputs(FILE_HEADER, leader);
			fclose(leader);
		}

		return(TRUE_VAL);
	}

#pragma endregion
