    #include "serverMain.h"

int usersLogged = 0;

int findUser(char * username, settings *s) {


	char buffer[TAMBUFFER];

	FILE * f = fopen(s->database, "r");
	if (f == NULL) {
		printf("Erro a abrir Ficheiro");
		return 2;
	}

	while (fgets(buffer, TAMBUFFER + 1, f) != NULL){
		buffer[strlen(buffer)-1] = 0;
		
		if (!strcmp(username, buffer)){
			fclose(f);
			return 1;
		}
	}

	printf("vou sair\n");

	fclose(f);

	return 0;
}


/*user recebeUser(char * path){			
	
	user u;

	int fd = open( path , O_RDONLY );
	if(fd == -1)
		printf("Erro a abrir o fifo %s \n", path);

	read(fd, u.nome, sizeof(char)*TAMBUFFER);
	read(fd, &u.pid, sizeof(int));

	printf("%d", u.pid);

	close(fd);
	return u;
}*/

void commands(settings * s) {


	char input[CMDSIZE], *cmd, *arg;
	int n;


	int fdw;
	int fdr;
	user novo;

	int logged = 0;

	user users[s->maxUsers];

	printf("before loop\n");

	do{
		fdr = open(s->mainPipe, O_RDONLY);
		if (fdr == -1)
			fprintf(stderr, "[ERROR] Can't read in ze pipe!\n");
		fdw = open(s->mainPipe, O_WRONLY);

		if (fdw == -1)
			fprintf(stderr, "[ERROR] Can't CENAS in ze pipe!\n");

		int r = read(fdr, &novo, sizeof(user));

		if(findUser(novo.nome, s))
			logged = 1;
		
		printf("\n %d \n", logged);

	}while(logged == 0);

	while (1) {

		printf("> ");

		fgets(input, CMDSIZE, stdin);

		cmd = strtok(input, " "); //TODO: Corrigir, só aceita comandos de uma palavra caso tenha espaço no fim.
		arg = strtok(NULL, " ");

		n = 0;

		while (cmd[n]) {
			cmd[n] = tolower((int)cmd[n]);
			n++;
		}

		printf("%s", cmd);

		if (!strcmp("settings", cmd)) {
			printf("\nText editor settings:\n");
			printf("Timeout: %d\n", s->timeout);
			printf("Maximum amount of pipes: %d\n", s->maxPipes);
			printf("Maximum amount of users: %d\n", s->maxUsers);
			printf("Maximum amount of lines: %d\n", s->e.max_l);
			printf("Maximum amount of columns: %d\n", s->e.max_c);
			printf("Name of the database: %s\n", s->database);
			printf("Name of the server pipe: %s\n", s->mainPipe);
			continue;
		}

		if (!strcmp("load", cmd)) {
			printf("\nYou've select 'load'.\n");
			continue;
		}

		if (!strcmp("save", cmd)) {
			printf("\nYou've select 'save'.\n");
			continue;
		}

		if (!strcmp("free", cmd)) {
			printf("\nYou've select 'free'.\n");
			continue;
		}

		if (!strcmp("statistics", cmd)) {
			printf("\nYou've selected 'statistics'.\n");
			continue;
		}

		if (!strcmp("users", cmd)) {
			printf("\nYou've select 'users'.\n");
			continue;
		}

		if (!strcmp("text", cmd)) {
			printf("\nYou've select 'text'.\n");
			continue;
		}

		if (!strcmp("shutdown", cmd)) {
			printf("\nYou've select 'shutdown'.\n");
			return;
		}

		if (strcmp("help", cmd) == 0) {
			printf("\nYou've selected 'help':\n");
			printf("'settings' will display the current system settings.\n");
			printf("'load <filename>' will load the content of <filename>.\n");
			printf("'save <filename>' will save what's being editted in <filename>.\n");
			printf("'free <linenumber>' erase contents of <linenumber>.\n");
			printf("'statistics' will display some statistics by the second whilst waiting for another command.\n");
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


	s = malloc(sizeof(settings));
		if(s == NULL)
			printf("Erro na alocação memoria para struct 'Settings' \n");


	initSettings(s, argc, argv, envp);


	if(access(s->mainPipe, F_OK) == 0){
        fprintf(stderr, "[ERROR] FIFO exists already!!\n");
        exit(1);}
	if(mkfifo(s->mainPipe, 0777)!=0) // 0666 read write a todos 0777 read write exe a todos
		fprintf(stderr, "[ERROR] FIFO couldn't be created!!\n");

	printf("fifo was created\n\n");

	//user novo = recebeUser(s->mainPipe);

	//TODO: verificar se existe o novo



	//unlink(cont char * filename); //Remove um FIFO/Ficheiro
	//fontl(int fd, int command, long arg); //Manipula as propriedades do FIFO/Ficheiro

	//open(filename, flags);
	//write(int fd, buffer, size_t size);
	//read(int fd, buffer, size_t size);


	//TODO finish the function below //(todo linha ~120 clienteMain.c)
	/*
	//receber username pelo serverPipe
	if(findUser(username, filename) == 1){
		//abrir novo pipe e passar ao cliente
	}else{
		//enviar erro ao cliente
	}
	*/

	printf("entering comands\n\n");
	commands(s);



	unlink(s->mainPipe);
	free(s);

	return EXIT_SUCCESS;
	
}