#ifndef LEIT_COMANDOS
#define LEIT_COMANDOS

//buffer de tamanho do username
#define TAMBUFFER 8

//defines de leitura de comandos
#define CMDSIZE 50
#define ARGSIZE 20

//medit defines
#define MEDIT_TIMEOUT 10
#define MEDIT_MAXUSERS 3
#define MEDIT_MAXLINES 15
#define MEDIT_MAXCOLUMNS 45
#define MEDIT_DEFAULT_NAME "medit.db"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <signal.h>

//---------------------------------------------------------------------------
//------------------------------Estruturas-----------------------------------

typedef struct ecra{
	int max_l;
	int max_c;
} ecra;

typedef struct settings {
	int timeout;
	int maxUsers;
	ecra e;
	char* database;
} settings;

typedef struct user{
    char nome[TAMBUFFER];
}user;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int findUser(char* user, char* filename);	// checks user FILE IS SPECIFIED BEFORE ENTERING HERE return 1 if finds 0 otherwise
void commands();
settings initSettings(settings s);

#endif