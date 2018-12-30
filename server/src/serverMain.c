  #include "serverMain.h"

int usersLogged = 0;
char pathSigint[20] = {""};

user users[MEDIT_MAXUSERS];//TODO: corrigir isto -vasco
/*
void inicializarTexto(textoCompleto *texto){//TODO: alterar frases!!!
		strcpy(texto->linha1, "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(texto->linha2, "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(texto->linha3, "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(texto->linha4, "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(texto->linha5, "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(texto->linha6, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha7, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha8, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha9, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha10, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha11, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha12, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha13, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha14, "Esta linha tem quarenta caracteres e pronto k");
		strcpy(texto->linha15, "Esta linha tem quarenta caracteres e pronto k");

}*/

void sigintHandler(int sig_num){
	//o valor de sig_num = 2A -tested
	signal(SIGINT, sigintHandler);
	printf("A encerrar tudo..");
	int i;
	for(i = 0; i< MEDIT_MAXUSERS; i++)
		//if(users[i] != -1)//TODO: get this to work //(if not null)
		kill(users[i].pid , SIGINT);
	
	
	unlink(pathSigint);
	fflush(stdout);

	exit(1);
}


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

void* commandsThread(void* args){

		
		settings *s = (settings *) args;
		

		char input[CMDSIZE], *cmd, *arg;
		int n;

		//printf("> ");

		fgets(input, CMDSIZE, stdin);

		cmd = strtok(input, " "); //TODO: Corrigir, só aceita comandos de uma palavra caso tenha espaço no fim.
		arg = strtok(NULL, " ");

		n = 0;

		while (cmd[n]) {
			cmd[n] = tolower((int)cmd[n]);
			n++;
		}
		

		//printf("%s", cmd);

		if (!strcmp("settings", cmd)) {
			printf("\nText editor settings:\n");
			printf("Timeout: %d\n", s->timeout);
			printf("Maximum amount of pipes: %d\n", s->maxPipes);
			printf("Maximum amount of users: %d\n", s->maxUsers);
			printf("Maximum amount of lines: %d\n", s->max_l);
			printf("Maximum amount of columns: %d\n", s->max_c);
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
			int i = 0;
			printf("\nYou've selected 'text'.");

			for(i = 0; i < MEDIT_MAXLINES; i++){
				//printf("\n %d \t %s", );//TODO Complete this all
			}
			printf("\n");


		}

		if (!strcmp("shutdown", cmd)) {
			printf("\nShutting down.");
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
	

		// not inside a circle, that's why it won't accept more than one command, I guess

		return s; //isto cala o warning de nao haver return (void *);



}


void server(settings * s/*, textoCompleto * textoServidor*/) {




	fd_set fontes;

	int fdw;
	int fdr;
	int fdfork1[2], fdfork2[2];	//unnamed pipes
	int r, w;
	user novo;

	int i;
	int pos_c = -1; //cliente
	int pos_l = -1; //livre
	char pathTemp[15];

	pthread_t threadCommands;

	char text[s->max_l][s->max_c];		//TODO: mudar isto para pointer pointer global e estatico
	//vale a pena?


	struct timeval tempo;


	tempo.tv_sec = s->timeout;
	tempo.tv_usec = 0;

	int logged = 0;
	int res;
	int forkSpell;


	int usersPID[s->maxUsers];
	for(i = 0; i< s->maxUsers; i++){
		usersPID[i] = -1;
	}

		fdr = open(s->mainPipe, O_RDWR);
		if (fdr == -1)
			fprintf(stderr, "[ERROR] Can't read pipe %s!\n", s->mainPipe);


		pthread_create(&threadCommands, NULL, commandsThread, (void *) s);





	do{
		logged = 0;//logged volta a 0
		FD_ZERO(&fontes);
		FD_SET(0, &fontes);	//teclado
		FD_SET(fdr, &fontes);	//fifo read
		

		tempo.tv_sec = s->timeout;
		tempo.tv_usec = 0;


		//todo timeout is working?
		res = select(fdr + 1, &fontes, NULL, NULL, &tempo);
		if(res==0){
			//printf("There's no data.\n"); 
			fflush(stdout);
		}
		if(res>0 && FD_ISSET(0, &fontes)){

		}

		if(res>0 && FD_ISSET(fdr, &fontes)){

			r = read(fdr, &novo, sizeof(user));

			if(r == 12){//user logged in

				printf("\tBytes: %d\tUSER: %s \tPID: %d\n",r, novo.nome, novo.pid);

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
			}else{
				if(r == 4){//server written int

				}
			}
			}
			if(logged == 0){
				// signal de kill cliente por nao existir username
				w = write(fdr, &logged, sizeof(int));
				if(w == 0)
					printf("nothing written");			
			}
			else{

				
				/**/
				w = write(fdr, &logged, sizeof(int));


				sleep(2);
				sprintf(pathTemp, FIFO_CLI, novo.pid);
				fdw = open(pathTemp, O_RDWR);
				w = write(fdw, &(*s) , sizeof(settings));
				if(w == 0)
					printf("nao consegui escrever no fifo do cliente");
					/*
				printf("\n\t\t BYTES TEXTO: %d\n\n", w);

					for(i = 0; i < MEDIT_MAXLINES; i++){
						

					}
					*/
				close(fdw);

				sleep(2);

				//logica fifo
				//for(i = 0; i<s->maxUsers; i++){
				//	if(usersPID[i] != -1 && usersPID[i] != novo.pid){
				//		//kill
				//		sprintf(fdw,FIFO_CLI,usersPID[i]);
				//	}
				//}

			}

		}
	//pthread_join(threadCommands, NULL);   //join thread com fim da função, sincronização da thread 

	}while(1);


	pthread_join(threadCommands, NULL);

		

}

void initSettings(settings * s, int argc, char * const argv[], char* envp[]) {

	char* aux = NULL;

	int flag;

	opterr = 0;

	

	//Inicializar valores por defeito
	s->timeout = MEDIT_TIMEOUT;
	s->maxUsers = MEDIT_MAXUSERS;
	s->maxPipes = MEDIT_MAXPIPES;
	s->max_c = MEDIT_MAXCOLUMNS;
	s->max_l = MEDIT_MAXLINES;
	//s->database = MEDIT_DEFAULT_NAME;
	strcpy(s->database, MEDIT_DEFAULT_NAME);
	//s->mainPipe = MEDIT_MAIN_PIPE_DEFAULT_NAME;
	strcpy(s->mainPipe, MEDIT_MAIN_PIPE_DEFAULT_NAME);

	//Substituir pelas variaveis de ambiente caso existam

	aux = NULL;
	aux = getenv("MEDIT_TIMEOUT");
	if (aux != NULL) {
		s->timeout = atoi(aux);
	}

	aux = NULL;
	aux = getenv("MEDIT_MAXLINES");
	if (aux != NULL) {
		s->max_l = atoi(aux);
	}

	aux = NULL;
	aux = getenv("MEDIT_MAXCOLUMNS");
	if (aux != NULL) {
		s->max_c = atoi(aux); 
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

void aspell(singleLine line){
	

	int fdfork1[2];
    int fdfork2[2];
    int forkSpell;
    char respostaDoFilho[100];
    char ret;

    long unsigned i;
	//remove this-----------start
    PEDIDO p;

    printf("Texto: "); fflush(stdout);
    scanf("%s", p.texto);

    printf("\nHere it is: %s\n", p.texto);
	//remove this...............end

	//TODO dividir frase da linha em palavras, testar palavra a palavra


    if(pipe(fdfork1) == -1)
		printf("ERROR CREATING PIPE!\n");
	if(pipe(fdfork2) == -1)
		printf("ERROR CREATING PIPE!\n");

	forkSpell = fork();

	if(forkSpell<0){
		printf("Error forking\n");
		exit(0);
	}
    else if(forkSpell > 0){	// pai
        
		close(fdfork1[0]);
        printf("%s", p.texto);
		write(fdfork1[1], p.texto, sizeof(p.texto));
		close(fdfork1[1]);
		wait(NULL); // espera pelo filho
        close(fdfork2[1]);
        strcpy(respostaDoFilho, " ");
        read(fdfork2[0], respostaDoFilho, sizeof(respostaDoFilho));
        for (i = 0; i < sizeof(respostaDoFilho); i++){
            if(respostaDoFilho[i] == '*'){
                printf("\nNice!!");
				line.isCorrect = 1;//set true
			}else if(respostaDoFilho[i] == '&'){
                  	printf("\nPalavra errada!!");
					line.isCorrect = 0;//set false
					}	
        }
            }
		else{		// filho
		    close(fdfork1[1]);
		    dup2(fdfork1[0], STDIN_FILENO);
		    close(fdfork1[0]);
		    close(fdfork2[0]);
			dup2(fdfork2[1], STDOUT_FILENO);
			close(fdfork2[1]);
			execlp("aspell", "aspell", "-a", NULL);
            }

}

int main(int argc, char * const argv[], char* envp[]) {

	signal(SIGINT, sigintHandler);



	settings *s;
	s = malloc(sizeof(settings));
		if(s == NULL)
			printf("Erro na alocação memoria para struct 'Settings' \n");
	/*
	textoCompleto *texto;
	texto = malloc(sizeof(textoCompleto));
		if(texto == NULL)
			printf("Erro na alocação memória para struct 'textoCompleto' \n");
	*/
	initSettings(s, argc, argv, envp);
	
	sprintf(pathSigint, s->mainPipe);//path do fifo na var. global
	/*
	inicializarTexto(texto);
	*/

	//canalização

	if(access(s->mainPipe, F_OK) == 0){//verifica se ja existe um servidor
        fprintf(stderr, "[ERROR] FIFO exists already!!\n");
        exit(1);}

	if(mkfifo(s->mainPipe, 0777)!=0) // 0666 read write a todos 0777 read write exe a todos
		fprintf(stderr, "[ERROR] FIFO couldn't be created!!\n");

	//main server function
	server(s/*, texto*/);


	

	//End of program

	int i;
	for(i = 0; i< MEDIT_MAXUSERS; i++)
		kill(users[i].pid , SIGINT);
	unlink(s->mainPipe);
	free(s);

	return EXIT_SUCCESS;
	
}