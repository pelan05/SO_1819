#ifndef LEIT_COMANDOS
#define LEIT_COMANDOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <signal.h>

#include "main.c"


//---------------------------------------------------------------------------
//------------------------------Estruturas-----------------------------------
typedef struct ecra{
    int MEDIT_MAXLINES;
    int MEDIT_MAXCOLUMNS;
} ecra;

typedef struct user{
    char nome[];
    char password[];

}user;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


#endif