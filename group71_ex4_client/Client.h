/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
 This file was written for instruction purposes for the 
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering, Winter 2011, 
 by Amnon Drory.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef SOCKET_EXAMPLE_CLIENT_H
#define SOCKET_EXAMPLE_CLIENT_H
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define TRUE_VAL 1
#define FALSE_VAL 0
#define MAX_NAME_SIZE 20
#define PRINT_TIMEOUT 100
#define eyal 100
#define PRINT_DENIE 200
#define PRINT_RETRY 300
#define SERVER_TIMEOUT 0x555

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

void clearScrean(char* code);
void MainClient(char* ip, char* port, char* name);
int closeClient();
int setName(char* nameToSet);
char* getClientName();
char* getIP_ADRESS();
int getPORT();

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#endif // SOCKET_EXAMPLE_CLIENT_H