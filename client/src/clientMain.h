#ifndef LEIT_COMANDOS
#define LEIT_COMANDOS


#define USERSIZE 8
#define TAMJANLINHASX 47
#define TAMJANY 17
#define TAMJANNOMESX (USERSIZE + 2) 
#define NUMLINHAS (TAMJANY-2)

#define MEDIT_MAXLINES 15
#define MEDIT_MAXCOLUMNS 45

#define FIFO_CLI "pipe_"

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
	int pid;
}user;


typedef struct linha{
	char *linha;
	int numLinha;
}singleLinha;

/*
typedef struct ecra {
	int max_l;
	int max_c;
} ecra;
typedef struct setting {
	int timeout;
	int maxUsers;
	int maxPipes;
	ecra e;
	char* database;
	char* mainPipe;
} settings;*/

typedef struct setting {
	int timeout;
	int maxUsers;
	int maxPipes;
	int max_l;
	int max_c;
	char database[20];
	char mainPipe[20];
} settings;

/*//deprecated
typedef struct textoCompleto{
	char linha1[45];
	char linha2[45];
	char linha3[45];
	char linha4[45];
	char linha5[45];
	char linha6[45];
	char linha7[45];
	char linha8[45];
	char linha9[45];
	char linha10[45];
	char linha11[45];
	char linha12[45];
	char linha13[45];
	char linha14[45];
	char linha15[45];
}textoCompleto;
*/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int verificaServidor(char * path);
int temEspaco(char * string);
int apagaCarat(char * string, int val);
int adicionaCarat(char * string, char adicao, int val);
void printLinhas(WINDOW *linhas, WINDOW *nomes, char **linha, int highlight);
void getUserEnv(int argc, char * const argv[], char * username, char * path);
void wPrintNumbers(WINDOW *numeros);
void string2FIFO(char * path, char * String, int * pid);


#endif