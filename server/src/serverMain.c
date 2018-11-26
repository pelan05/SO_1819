#include "serverMain.h"


int findUser(char* user, char* filename) {


	char buffer[TAMBUFFER];

	FILE * f = fopen(filename, "r");
	if (f == NULL) {
		printf("Erro a abrir Ficheiro");
		return 2;
	}

	while (fgets(buffer, sizeof(buffer), f) != NULL)
		if (!strcmp(user, buffer))
			return 1;

	return 0;
}

void commands(settings *s) {


	char cmd[CMDSIZE], *arg[1];
	int n;//, i;
		  //int argSize;
	while (1) {

		printf("> ");

		fgets(cmd, CMDSIZE, stdin);

		n = 0;
		arg[n] = strtok(cmd, " ");
		n++;
		while ((arg[n] = strtok(NULL, " ")) != NULL)
			n++;

		n = 0;

		while (arg[n]) {
			tolower((int)arg[n]);
			n++;
		}


		if (!strcmp("settings", arg[0])) {
			printf("Text editor settings:\n");
			printf("Timeout: %d\n", s->timeout);
			printf("Maximum amount of pipes: %d\n", s->maxPipes);
			printf("Maximum amount of users: %d\n", s->maxUsers);
			printf("Maximum amount of lines: %d\n", s->e.max_l);
			printf("Maximum amount of columns: %d\n", s->e.max_c);
			printf("Name of the database: %s", s->database);
			printf("Name of the server pipe: %s", s->mainPipe);
			continue;
		}

		if (!strcmp("load", arg[0])) {
			printf("You've select 'load'.\n");
			continue;
		}

		if (!strcmp("save", arg[0])) {
			printf("You've select 'save'.\n");
			continue;
		}

		if (!strcmp("free", arg[0])) {
			printf("You've select 'free'.\n");
			continue;
		}

		if (!strcmp("statistics", arg[0])) {
			printf("You've selected 'statistics'.\n");
			continue;
		}

		if (!strcmp("users", arg[0])) {
			printf("You've select 'users'.\n");
			continue;
		}

		if (!strcmp("text", arg[0])) {
			printf("You've select 'text'.\n");
			continue;
		}

		if (!strcmp("shutdown", arg[0])) {
			printf("You've select 'shutdown'.\n");
			abort();
		}

		if (!strcmp("help", arg[0])) {
			printf("You've selected 'help':\n");
			printf("'settings' will display the current system settings.\n");
			printf("'load <filename>' will load the content of <filename>.\n");
			printf("'save <filename>' will save what's being editted in <filename>.\n");
			printf("'free <linenumber>' erase contents of <linenumber>.\n");
			printf("'statistics' will display some statistics by the second while waiting for another command.\n");
			printf("'users' will a list of what users are logged in, by order of arrival.\n");
			printf("'text' will the current text on client side.\n");
			printf("'shutdown' will quit without saving.\n");
			continue;
		}
	}

}

void initSettings(settings * s, int argc, char * const argv[], char* envp[]) {

	char* aux = NULL;

	int flag;

	opterr = 0;

	

	//Inicializar valores por defeito
	s->timeout = MEDIT_TIMEOUT;
	s->maxUsers = MEDIT_MAXUSERS;
	s->maxPipes = MEDIT_MAXPIPES;
	s->e.max_c = MEDIT_MAXCOLUMNS;
	s->e.max_l = MEDIT_MAXLINES;
	s->database = MEDIT_DEFAULT_NAME;
	s->mainPipe = MEDIT_MAIN_PIPE_DEFAULT_NAME;

	//Substituir pelas variaveis de ambiente caso existam

	aux = NULL;
	aux = getenv("MEDIT_TIMEOUT");
	if (aux != NULL) {
		s->timeout = atoi(aux);
	}

	aux = NULL;
	aux = getenv("MEDIT_MAXLINES");
	if (aux != NULL) {
		s->e.max_l = atoi(aux);
	}

	aux = NULL;
	aux = getenv("MEDIT_MAXCOLUMNS");
	if (aux != NULL) {
		s->e.max_c = atoi(aux);
	}

	aux = NULL;
	aux = getenv("MEDIT_USERS");
	if (aux != NULL) {
		s->maxUsers = atoi(aux);
	}


	while ((flag = getopt(argc, argv, "f:p:n:")) != -1)
		switch (flag) {
		case 'f':
			strcpy(s->database, optarg);
			break;
		case 'p':
			strcpy(s->mainPipe, optarg);
			break;
		case 'n':
			s->maxUsers = atoi(optarg);
			break;
		case '?':
			if (optopt == 'f')
				fprintf(stderr, "Option -%c requires an argument, the filename.\n", optopt);
			else if (optopt == 'p')
				fprintf(stderr, "Option -%c requires an argument, the name for the pipe.\n", optopt);
			else if (optopt == 'n')
				fprintf(stderr, "Option -%c requires an argument, the number of pipes.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option -%c.\n", optopt);
			else
				fprintf(stderr, "Unknown option character \\x%x.\n", optopt);
		}


}


int main(int argc, char * const argv[], char* envp[]) {

	settings *s;

	printf("Debugger 1\n");
	s = malloc(sizeof(settings));
		if(s == NULL)
			printf("Erro na alocação mem linha 188 serverMain.c");

	printf("Debugger 2\n");

	initSettings(s, argc, argv, envp);

	printf("Debugger 3\n");
	commands(s);


	printf("Debugger 4\n");

	free(s);
	return EXIT_SUCCESS;

}