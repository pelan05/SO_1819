#ifndef LEIT_COMANDOS
#define LEIT_COMANDOS

//buffer de tamanho do username
#DEFINE TAMBUFFER 8

//defines de leitura de comandos
#DEFINE CMDSIZE 50
#DEFINE ARGSIZE 20

//medit defines
#define MEDIT_TIMEOUT 10
#define MEDIT_MAXLINES 15
#define MEDIT_MAXCOLUMNS 45

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
    int max_l = MEDIT_MAXLINES;
    int max_c = MEDIT_MAXCOLUMNS;
} ecra;

typedef struct user{
    char nome[TAMBUFFER];
}user;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


#endif