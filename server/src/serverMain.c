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


	fclose(f);

	return 0;
}

void* commandsThread(void *args){

printf("Entrei no commandsthread 1");

		settings *s;
		s = (settings *) &args;

printf("commandsthread 2");
		char input[CMDSIZE], *cmd, *arg;
		int n;

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

printf("commandsthread 3");
		if (!strcmp("settings", cmd)) {
			printf("\nText editor settings:\n");
			printf("Timeout: %d\n", s->timeout);
			printf("Maximum amount of pipes: %d\n", s->maxPipes);
			printf("Maximum amount of users: %d\n", s->maxUsers);
			printf("Maximum amount of lines: %d\n", s->e.max_l);
			printf("Maximum amount of columns: %d\n", s->e.max_c);
			printf("Name of the database: %s\n", s->database);
			printf("Name of the server pipe: %s\n", s->mainPipe);
		}

		if (!strcmp("load", cmd)) {
			printf("\nYou've select 'load'.\n");
		}

		if (!strcmp("save", cmd)) {
			printf("\nYou've select 'save'.\n");
		}

		if (!strcmp("free", cmd)) {
			printf("\nYou've select 'free'.\n");
		}

		if (!strcmp("statistics", cmd)) {
			printf("\nYou've selected 'statistics'.\n");
		}

		if (!strcmp("users", cmd)) {
			printf("\nYou've select 'users'.\n");
		}

		if (!strcmp("text", cmd)) {
			printf("\nYou've select 'text'.\n");
		}

		if (!strcmp("shutdown", cmd)) {
			printf("\nYou've select 'shutdown'.\n");
			unlink(s->mainPipe);
			exit(EXIT_SUCCESS);//'0'
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
		}
	



		return s; //isto cala o warning de nao haver return (void *);

}


void server(settings * s) {



	fd_set fontes;

	int fdw;
	int fdr;
	int fdg;
	int r, w;
	user novo;

	int i;
	int pos_c = -1; //cliente
	int pos_l = -1; //livre
	char pathTemp[15];

	pthread_t threadCommands;


	struct timeval tempo;


	tempo.tv_sec = s->timeout;
	tempo.tv_usec = 0;

	int logged = 0;
	int res;

	user users[s->maxUsers];

	int usersPID[s->maxUsers];
	for(i = 0; i< s->maxUsers; i++){
		usersPID[i] = -1;
	}

		fdr = open(s->mainPipe, O_RDONLY);
		if (fdr == -1)
			fprintf(stderr, "[ERROR] Can't read pipe %s!\n", s->mainPipe);
		fdg = open(s->mainPipe, O_WRONLY);
		if (fdg == -1)
			fprintf(stderr, "[ERROR] Can't write pipe %s!\n", s->mainPipe);
	do{
		FD_ZERO(&fontes);
		FD_SET(0, &fontes);	//teclado
		FD_SET(fdr, &fontes);	//fifo read
		
		res = select(fdr + 1, &fontes, NULL, NULL, &tempo);
		if(res==0){
			printf("There's no data.\n"); fflush(stdout);
		}
		if(res>0 && FD_ISSET(0, &fontes)){//comandos (cria thread para correr 'commandsThread() ao mesmo tempo que le fifo)

		//thread  que corre a função commandsThread();

		
		printf("antes ptread");

		pthread_create(&threadCommands, NULL, commandsThread, s);


		pthread_join(threadCommands, NULL); 




		}

		if(res>0 && FD_ISSET(fdr, &fontes)){

			r = read(fdr, &novo, sizeof(user));


			printf("\n\tBytes: %d\tUSER: %s \tPID: %d\n",r, novo.nome, novo.pid);

			if(findUser(novo.nome, s)){

				logged = 1; //se login com sucesso, var logged fica a 1
				pos_c = -1; //posiçao cliente
				pos_l = -1; //posição livre


				//TODO se o array estiver cheio? (sig de maximo de clientes conectados)
				for(i = 0; i < s->maxUsers; i++){
					if(usersPID[i] == -1 && pos_l == -1)// primeira posiçao livre encontrada 
						pos_l = i;
					if(usersPID[i] == novo.pid)	// se user encontrado
						pos_c = i;
				}
				if(pos_c == -1 && pos_l != -1){		// substituir posiçao livre por user recebido
					usersPID[pos_l] = novo.pid;
					users[pos_l] = novo;
				}
			}
			if(logged == 0){
			
				// signal de kill cliente por nao existir username

			
			}
			else{
				printf("Client logged in\n\n");
				sprintf(pathTemp, FIFO_CLI, novo.pid);
				fdw = open(pathTemp, O_WRONLY);
				w = write(fdw, &novo, sizeof(user));
				printf("\n\tBytes written: %d\n", w);
				close(fdw);




				//logica fifo
				//for(i = 0; i<s->maxUsers; i++){
				//	if(usersPID[i] != -1 && usersPID[i] != novo.pid){
				//		//kill
				//		sprintf(fdw,FIFO_CLI,usersPID[i]);
				//	}
				//}

			}
		
		}
		printf("\nlogged: %d \n", logged);


	//pthread_join(threadCommands, NULL);   //join thread com fim da função, sincronização da thread 

	}while(1);



		

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

	server(s);


	

	//End of program
	unlink(s->mainPipe);
	free(s);

	return EXIT_SUCCESS;
	
}