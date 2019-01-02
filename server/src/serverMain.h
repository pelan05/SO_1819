#ifndef LEIT_COMANDOS
#define LEIT_COMANDOS

//buffer de tamanho do username
#define TAMBUFFER 8

//defines de leitura de comandos
#define CMDSIZE 50

//define de tamanho de filename
#define MAXFILENAME 20

//medit defines
#define MEDIT_TIMEOUT 10
#define MEDIT_MAXPIPES 3
#define MEDIT_MAXUSERS 3
#define MEDIT_MAXLINES 15
#define MEDIT_MAXCOLUMNS 45
#define MEDIT_DEFAULT_NAME "medit.db"
#define MEDIT_MAIN_PIPE_DEFAULT_NAME "serverPipe"

#define FIFO_CLI "pipe_%d"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/wait.h>


//---------------------------------------------------------------------------
//------------------------------Estruturas-----------------------------------

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

typedef struct pedido{
    char texto[45];
    int rem;
}PEDIDO;

typedef struct user {
	char nome[TAMBUFFER];
	int pid;
}user;

typedef struct linha{
	//num linha
	int lineID;
	//editada ou n
	int isBeingEdited;
	//correta ou nao
	int isCorrect;
	//texto
	char *text;
}singleLine;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//void inicializarTexto(textoCompleto *texto);//inicializa o texto do server side com texto default
int findUser(char * username, settings *s);	// checks user FILE IS SPECIFIED BEFORE ENTERING HERE return 1 if finds 0 otherwise
void* commandsThread(void *args);
void server(settings * s/*, textoCompleto * textoServidor*/);
void initSettings(settings *s, int argc, char * const argv[], char* envp[]);
int aspell(singleLine line);
void saveInFile(char *arg, settings *s);

#endif