//
//  DefenitionsForServerClient.h
//  Ex04_Server
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#ifndef DefenitionsForServerClient_h
#define DefenitionsForServerClient_h

#pragma region MessegesDefine

	#pragma region Client
	#define CLIENT_REQUEST "CLIENT_REQUEST"
	#define CLIENT_MAIN_MENU "CLIENT_MAIN_MENU"
	#define CLIENT_CPU "CLIENT_CPU"
	#define CLIENT_VERSUS "CLIENT_VERSUS"
	#define CLIENT_LEADERBOARD "CLIENT_LEADERBOARD"
	#define CLIENT_PLAYER_MOVE "CLIENT_PLAYER_MOVE"
	#define CLIENT_REPLAY "CLIENT_REPLAY"
	#define CLIENT_REFRESH "CLIENT_REFRESH"
	#define CLIENT_DISCONNECT "CLIENT_DISCONNECT"
	#define CLIENT_SHELDON "CLIENT_SHELDON"
#pragma endregion

	#pragma region Server

	#define SERVER_MAIN_MENU "SERVER_MAIN_MENU"
	#define SERVER_APPROVED "SERVER_APPROVED"
	#define SERVER_DENIED "SERVER_DENIED"
	#define SERVER_INVITE "SERVER_INVITE"
	#define SERVER_PLAYER_MOVE_REQUEST "SERVER_PLAYER_MOVE_REQUEST"
	#define SERVER_GAME_RESULTS "SERVER_GAME_RESULTS"
	#define SERVER_GAME_OVER_MENU "SERVER_GAME_OVER_MENU"
	#define SERVER_OPPONENT_QUIT "SERVER_OPPONENT_QUIT"
	#define SERVER_NO_OPPONENTS "SERVER_NO_OPPONENTS"
	#define SERVER_LEADERBOARD "SERVER_LEADERBOARD"
	#define SERVER_LEADERBORAD_MENU "SERVER_LEADERBORAD_MENU"
	#define SERVER_SHELDON "SERVER_SHELDON"
	#define SERVER_BAZINGA "SERVER_BAZINGA"
	#define SERVER_SPOCK "SERVER_SPOCK"
	#define SERVER_KEEP_ALIVE "SERVER_KEEP_ALIVE"

#pragma endregion

#pragma endregion

#pragma region Define

	#pragma region Strings

		#define IPCONFIG_STRING "   IPv4 Address. . . . . . . . . . . "
		#define UNSUPPURTED_MESSAGE "this message type is unsupported in our system.\n"
		#define SERVER_NO_OPPONENT_MSG "there is no opponent avilable.\n\n"
		#define ROCK "ROCK"
		#define PAPER "PAPER"
		#define SCISSORS "SCISSORS"
		#define LIZARD "LIZARD"
		#define SPOCK "SPOCK"
		#define DREW_IN_GAME "DRAW"
		#define PARAMETER_BEGIN_CHAR ":"
		#define PARAMETER_SPLIT_CHAR ";"
		#define SHELDON "                           .((#@&&&%&@@@@%&%#(*                               \n                          .((%&&&&%&&@@@@@@@&&@%((                             \n                         /(#&&%&%&&%&@@@@@@%@@&#(.                           \n                        .(%&&&(///***/(%#((((%%@%&&(                           \n                        (#%//***********///(#%%&%#/                          \n                        //#%*******,,,,,,**///(#%&%/(.                         \n                        /*## *****,,,,,,,***//*(%%#*(.                         \n                        //(,.*****,,,,,,,***//*,(#(/(.                         \n                        ///  *//**,,,,,,,*/(((,. %%*(.                         \n                        .// ,((/#%#**,,*(%#/(((* /#*(.                         \n                         ,/ ,///%(**/,*//.#(#(/, ((*(                          \n                         ,,,,****,,**,*/*,****/..(*//                          \n                          .* ,*,,*,**,*/*,,,***..*//                           \n                           *, *,,**/*,*//,,,**,,,/,                            \n                            ,***/*********/////*/*                             \n                             //*,*#(/**/((*,//((/.                             \n                              ///**/////***/(%%(/.                             \n                              /*(****,,,*//#%((/*/.                            \n                              /,/(#//**/(%##/*//,*%#((*                        \n                            ./*,////((((/(//***//,//%%##(//,                   \n                       .///((/*,/*///*,,******///(((##%%%%%%%#//*              \n                   ,////######*(****////*****/***/(##%%%%%%%%%%%%%#//,         \n                 ////##%%%%###(/*,,**/********,,,*##%%%%%%%%%%%%%%%%#(/*       \n             .///(#%%%%%%%%%####*,.,,,,,*,,,,,*/#############%%%%%%%%%#//      \n           *///(%%%##%##%###########/,,,..*(#################%%%%%%%%%%#(/     \n          ///(#%%#################################################%%%%%%#(/    \n         ///(#%%#############################################%######%%%%%#(/.  \n        *//(#####################################(###########%#####%%%%%%%#(/  \n        /*(##%##################################/############%##%##%#%%%%%%#(/ \n       //((%%######%%##########################*(##############%#%%&%%%%%%%%#( \n      .//#(%%%%#%##%%##%%################(/(#**###############%%%%%%%%%%%%%%%# \n      //(#%#%%%#%%%%#%%###########(**//***,,../*(############%#%%%%%%%%%%%%%%% \n     ,//#%%%%%%%%%%#%%###########/*,......,..*.,,,*###########%%%%%%&%&%%%%%%% \n     //##%%%#%%%%%%%%##########..........*../,*(....*#%######%%%&&%&&&%%&%%%%% \n    //(#%%%%%%%%%%##########...  . . /......(. .....####%%%%%%%%&&&%%&%%%%%* \n   **,##%%%%%%&%&&%%%#%#####%. ..  .../......# .. ....,%#%%%%%%%%%%&%%%(,,,*#% \n   /./.*%%%%%%%%&%&&&%%#####(. .....*#.(..../..........%##%%%%%%%%%(**(%%%,.,, \n   /(,,##/,,,,****/#&%%%####* ........(..../ .,........#%%%%%%%%%%&&/*,,/#%%%% \n  *(%%%#/.,,,,**///%%%%%%%##%......../....../.........,#%%%%%%%%%%%%%%%%%%#% \n  ,#%%%%%###%%%%%%%&&&%%%%%%%*......(....../..........%#%%%%%%%%%%&%%%%%%%%%&% \n *,/((#%%%%#%%%%%%%%%%%%%%%%%%(/.../,*(...*........./###%%%%%%%%%%%&&(##(((((( \n ,*(((((((((#####(###%%%%&%%%%%#//.../..*.........,(###%%%%%%%%%%%&&%((###(((( \n.,*(((((((((((((#/#%%%%%%%#%%%%%%#/*/,,/........,*(####%%%%%%%%%%&&&%%(#####(( \n/,/(((((((((((((///##%%%%%%%%%%%%%%#*/////////(#########%%%%%%%%%&%%%%%####### \n/,/(/(///(/////////(#%%%%#%%%%%%%%#*#%%%####%%%%#######%%%%%%%%%&%&%%%%%((#### \n/*(/(((((/((((*////(&%%%%%%#%%%%%#*%#####%###########%#%%%%%%%%&&%%%%%%%(((#%# \n/*///((//(((((/*///(%%%%%%%%%%%%##%%%%%%%%########%%%%%%%%%%%%&&%%%%%%%%%((#(# \nBazinga! you have fallen for another one of my practical jokes!!\n"
		#define SHELDON_SPOCK "                              *%&&&&&&(                                      \n                            (&%%#####%%%&&%%%/                                 \n                          (&%%%%%%##%&&&&&%%%/.                                \n                        #&&&&&&&&&&%%%&%@@&&.                                \n                      .&&%%%&&&&&&@@&&&%&&&&&&%,                               \n                      ,&&%%%%&@&@@&%(/,,&%%%%%%#                               \n                      #&&&&%%#(//*******%&&&&%%(..                             \n                     ,&&@%/*,,**********(&@@@@,,                             \n                    .%&&%******//////////#&@@@@#/,                             \n                    %%&&%******///////////(%@#(.                             \n                    (%%&%#%#(////(#%&%%#(//(%(%#.                              \n                     *(((/##%%#(#%%%%%%#((/(#(/.                               \n                      .//**///**(###((////((((/                                \n                        */*******/((///////((*                                 \n           */*           ./(#(*//(######(((((/.                                \n  .,      (/(/           *#(/(((##%%#####((//#(,                             \n  ./*(*   #&/((/.           ///((#%%####%#(((//*/#%&&(,                        \n   ///#(  (#*/(/(.          .#(*///(((#%#((((//*/##((((#%%/..                  \n  .*//*#*  //(//(,      .,((#(((/((#%%%#((//////####(##(((/*#%(.               \n  ((/(//#  (**#*,,(%&&%(//(#%#((///(###(((((%%%######%#(##(//((*.            \n  (#//#(//*****/%&&%(((((##%%(##(########%%%%%%%#####%%#%%#(#((((///*          \n  ,%//*///*/**/*%##(//(#&%#(#(##############%%%%%###%%#%%#######((//         \n  ,&///////***//(###*/(#%&%#(#########################%%############(((.       \n   (/**///**********(##%%#############################%###%#####%%####(#,      \n   (//*************/#(%%##############################%####%###%%%#%%##(#.     \n   ##*//********//###%%##%%#############%%%%%#########%%######%%##%%%%##(.     \n  *&%(//////////#%%#%#/##(/(((((#######%%%%%%###(((//**##%%###%#%%%%%%%##,     \n  *((/(((((##%%%%%%%%%%&&%&&%%##(//////////((((#%&&&@(/%%%&%%%%%%%%%%%%%%(     \n,#%%*/(/(####%%%&%&&/#&**(/*,*/(####%%%%%###(/*,(/#%%%&&%&%%%%%%%%%%%#*    \n  .,((**/((#(#%%%&&&%%#/&(*/**,//*/*/((#/((#//#*,(&%*(##%%%&&#%%%&%%%%%##/   \n/(###////((((#%%%&&&%%#((%#*/**/**/*//(/(//(*(*/#/#%%&&&&&&%%&%&%%%%%%%%##*  \n,,,**///(((#(%@@@@@@&%##/%%%%%&&&&&&&&&&&&&&&&&&@#/#%%%%&&&&&%%&%%%%%%%%%%%%&* \n(((((//((((((&@@@@@@@#((&%%%%&%#(((((#&&&&%%%%#(#*. ......        ./%&&&&@*\n%%%#(//(((((/%&&@@@@@@%%#(#&&&(#%%%%#((%&&%&&(*(&%/////********//((#####&&&@#\n%%%((((((#((((&@@@@@@@@%%%%%%&%##%%&&&%%(#%%%%(#*.../#&&&&&&&@&&&&&%%%###%&&&\n (%((((####((/%@@@@@@@@@%&&%%@&((#%%&&&%##&@%#%&(,,,.*&&*,,,,.,,,,,,,,,,*&&&&\n #@%((######((&@@@@@@&@@@&&&&&%((((((/#&&/(%%%%//*,/,,,,,,,,,,,,,,,*#&&&&\n .&&%###%%###&@@@@@@*@@@@@&&&&%%@&&&&%%%@(/#%%&&&(**/%&*,////(((/(/*%&&&&&&&,\n  %@@@@&&&@@@@@@@@@& %@&&@@&&&&%(%@%%%&&/#%%%%%%&&,/&%%&&&,*/(((**%&&&&&&&&( \n  *@@@@@@@@@@@@@@@.  ,@&&@@&&&&&&&@@##&&&&&&%%%(#&&%%&&&(/&%/,,/%&(&&&&&*  \n   #@@@@@@@@@@@@*    .%%&&&@&&&&%%%%%%%%%%#%#%%*((#%%&&%##%%%#####%%&%&&&&,  \n   ,%@@@@@@@@@%.     /%%&&&&&&&%%%%%%%%%%%%%%&%%( ../%%&&%&%%%%%%%%&@%&&&@(  \n     /&@@@@%(,      *#%%&&&&%%&&&&&&%%%%%%%&&&&.....#&&%&@##%%#%%%%&&%&&&@&. \n      ,(((((*     .,#%%%#%&#%%%%##%%&&&&&&%%#.*#,.#%%%&#%(/(%%%&&%&%&@@* \n                *((#%#%%&@@%%#//%%###%%########%(.*%(,#%%%&@%##((#(#%&&%%%&@#  \n               ,./%%&#&%###(*/%%%&&&&&&&&%%%%%#.,%(,#%%%&@%#(/*/((%&&%%%*    \n              /&%%#//((/(((//#&&&&&&%%%%##%%%%# ..*,(%%%&&%%#(/((#%&&%,      \n              #@&&%#(#(########%%%#%#%%%%%%%&&&... .(%%#&#/(*/(##*         \n                            ,((#&@&&%%%%%%%%%%%%#.,, .(%##&%%#(,,            \n                                        *#%&&&%%(../(,/###&&/,.                \n"

#pragma endregion

	#pragma region Errors

	#pragma region ErrorCodes

	#define NO_ERROR_VAL 1
	#define GENERAL_ERROR -100
	#define DISCONNECTED_FROM_SERVER -200
	#define INPUT_ERROR -300
	#define MALLOC_ERROR -400
	#define ERROR_IN_CONNECTION -500
	#define FILE_READ_ERROR -600
	#define NO_PARAM_ERROR -700
	#define THREAD_HANDLE_ERROR -800
	#define MUTEX_ERROR -900
	#define THREAD_WAIT_ERROR -1000
	#define THREAD_TERMINATE_ERROR -1100
	#define THREAD_ERROR -1200
	#define	MAIN_SOCKET_ERROR -1300

	#pragma endregion
	
	#pragma region Messages

		#define GENERAL_ERROR_MSG "There was a general error in the system.\n"
		#define DISCONNECT_FROM_SERVER_MSSG "Disconnected from server.\n"
		#define INPUT_ERROR_MSSG "There was a problem with one of the inputs.\n"
		#define MALLOC_ERROR_MSG "There was a problem mallocing mem.\n"		
		#define ERROR_IN_CONNECTION_ERROR "There was a problem opening one of the files.\n"
		#define FILE_READ_ERROR_MSG "There was a problem opening one of the files.\n"
		#define NO_PARAM_ERROR_MSG "Not enough params error.\n"
		#define THREAD_HANDLE_ERROR_MSG "There was a problem in handleing the thread.\n"
		#define MUTEX_ERROR_MSG "There was a problem on mutex/semaphore.\n"
		#define THREAD_WAIT_ERROR_MSG "There was a problem waiting for the thread.\n"
		#define THREAD_TERMINATE_ERROR_MSG "There was a problem terminating the thread.\n"
		#define THREAD_ERROR_MSG "There was a problem in the thread.\n"
		#define SERVER_QUIT_MESSAGE "the %s has left the game, return to main menue.\n\n"
		#define	MAIN_SOCKET_ERROR_MESSAGE "The main socket didnt close well.\n"

	#pragma endregion


#pragma endregion

	#pragma region Numbers
		#define CONTINUE_RUN 22
		#define QUIT_CLIENT 23
		#define DISCONNECT_TRY_CONNECT 24
		#define SERVER_TIMEOUT 0x555
		#define FALSE_VAL 0
		#define TRUE_VAL 1
		#define MAX_IP_SIZE 200
	#pragma endregion

#pragma endregion

#pragma region Function

	void errorPrinter(int exitCode);

#pragma endregion

#endif /* DefenitionsForServerClient_h */
