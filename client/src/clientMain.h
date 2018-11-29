#ifndef LEIT_COMANDOS
#define LEIT_COMANDOS


#define USERSIZE 8
#define TAMJANLINHASX 47
#define TAMJANY 17
#define TAMJANNOMESX (USERSIZE + 2) 
#define NUMLINHAS (TAMJANY-2)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <pthread.h>
#include <ncurses.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//---------------------------------------------------------------------------
//------------------------------Estruturas-----------------------------------


typedef struct user {
	char nome[USERSIZE];
}user;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int verificaServidor(char * path);
int temEspaco(char * string);
int apagaCarat(char * string, int val);
int adicionaCarat(char * string, char adicao, int val);
void printLinhas(WINDOW *linhas, WINDOW *nomes, char **linha, int highlight);
void getUserEnv(int argc, char * const argv[], char * username, char * path);
void pedeUser(char * username);
void wPrintNumbers(WINDOW *numeros);
int usernameExists(char * username);
/*
char * leFifo(char * path, char * String);
*/
void olaFifo(char * path, char * String, int pid);


#endif