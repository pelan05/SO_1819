  #include "serverMain.h"

int usersLogged = 0;
char pathSigint[20] = {""};
singleLine *texto;
settings *s;
user *users; //TODO: corrigir isto -vasco
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
	printf("\nA encerrar tudo...\n");
	int i;
	for(i = 0; i< s->maxUsers; i++)
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

		do{
		settings *s = (settings *) args;
		

		char input[CMDSIZE], *cmd, *arg;
		int n;

		//printf("> ");

		fgets(input, CMDSIZE, stdin);

		cmd = strtok(input, " "); 
		arg = strtok(NULL, " ");

		if(arg == NULL)
			cmd[strlen(cmd)-1] = '\0';


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
		} else

		if (!strcmp("load", cmd)) {
			printf("\nYou've selected 'load'.\n");
		} else

		if (!strcmp("save", cmd)) {
			printf("\nYou've selected 'save'.\n");
			saveInFile(arg, s);
		} else

		if (!strcmp("free", cmd)) {
			printf("\nYou've selected 'free'.\n");
			int lineVal = atoi(arg);	//retorna 0 caso seja inválido
			if(lineVal <= s->max_l)
				freeLine(lineVal);		//se passar 1 apaga a primeira linha, etc
			else
				printf("\nInvalid argument for 'free'!\n");
		} else

		if (!strcmp("statistics", cmd)) {
			printf("\nYou've selected 'statistics'.\n");
		} else

		if (!strcmp("users", cmd)) {
			printf("\nYou've selected 'users'.\n");
			listUsers();
		} else

		if (!strcmp("text", cmd)) {
			printf("\nYou've selected 'text'.\n");
			printText();


		} else

		if (!strcmp("shutdown", cmd)) {
			printf("\nShutting down.");
			kill(getpid(), SIGINT);
		} else

		if (!strcmp("aspell", cmd)) {
			if (aspell(arg) == 1)
				printf("\nCorrect!\n");
			else
				printf("\nIncorrect\n");
		} else

		if (!strcmp("help", cmd)) {
			printf("You've selected 'help':\n");
			printf("'settings' will display the current system settings.\n");
			printf("'load <filename>' will load the content of <filename>.\n");
			printf("'save <filename>' will save what's being editted in <filename>.\n");
			printf("'free <linenumber>' erase contents of <linenumber>.\n");
			printf("'statistics' will display some statistics by the second whilst waiting for another command.\n");
			printf("'users' will a list of what users are logged in, by order of arrival.\n");
			printf("'text' will the current text on client side.\n");
			printf("'shutdown' will quit without saving.\n");
		} else{
			printf("INVALID COMMAND!\n");
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

		}while(1);

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


//		pthread_create(&threadCommands, NULL, commandsThread, (void *) s);





	do{
		//todo HERE
		//kill(pid, 0);
		for(i = 0; i < MEDIT_MAXUSERS; i++){
			if(users[i].pid != NULL){				//se user está no array
				if(kill(users[i].pid, 0) != 0){		//se user não está ativo (==0 é ativo)
					strcpy(users[i].nome, "");
					users[i].pid = NULL;
					usersLogged--;
				}
			}
		}



		pthread_create(&threadCommands, NULL, commandsThread, (void *) s);




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


				if(findUser(novo.nome, s) && usersLogged < 3){


					printf("\tBytes: %d\tUSER: %s \tPID: %d\n",r, novo.nome, novo.pid);
					usersLogged++;
					
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


	//pthread_join(threadCommands, NULL);

		

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

int aspell(char *word){
	
	int fdfork1[2];
    int fdfork2[2];
    int forkSpell;
    char respostaDoFilho[100];
    char ret;
	char aux[MEDIT_MAXCOLUMNS/*s->max_c*/]; // isto tem de ser com o valor de max columns das settings

    long unsigned i;

	//TODO usar o que está abaixo efetivamente dentro do esquema de aspell
/*
	strcpy(aux, word);

	char *ptr = strtok(aux, delim);	//separa a primeira palavra

	while (ptr != NULL){			//separa as restantes palavras
		ptr = strtok(NULL, delim);
	}
*/
	//		words separating ends here

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
        printf("%s", word);
		write(fdfork1[1], word, sizeof(word));
		close(fdfork1[1]);
		wait(NULL); // espera pelo filho 
        close(fdfork2[1]);
        strcpy(respostaDoFilho, " ");
        read(fdfork2[0], respostaDoFilho, sizeof(respostaDoFilho));
			for(i = 0; i < sizeof(respostaDoFilho); i++)
				if(respostaDoFilho[i] == '*'){
					//printf("\nNice!!");
					return 1;
				}else if(respostaDoFilho[i] == '&'){
						//printf("\nPalavra errada!!");
						return 0;
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

void saveInFile(char *arg, settings *s)
{

	int i;
	
	FILE *f = fopen(arg, "w");
	if (f == NULL){
		printf("Error opening file!\n");
		return;
	}
	
	for(i = 0; i < s->max_l; i++)
		//TODO: Checkar a linha com o aspell e só guardar no ficheiro se der bem (needs testing only)

		/*if(aspell(texto[i])==1){
			fprintf(f, "%s\n", texto[i].text);
		}
		else{
			fprintf(f, "\n");
		}*/
		
		
		
		
	fclose(f);
}



void printText(){
	int i;

	for (i=0; i<s->max_l; i++){
		printf("%s", texto[i].text);
	}
}

void freeLine(int val){

	int i, j;

	for(i = 0; i < s->max_l; i++){
		if(i+1 == val)
			for(j=0; j< s->max_c; j++)
				texto[i].text[j] = 0; 
	}
}

void listUsers(){
	int i;

	if(usersLogged == 0){
		printf("There are no active users.");
	}else{
		printf("Printing Active Users: \n");
		for(i = 0; i < MEDIT_MAXUSERS; i++){
			if(users[i].pid != NULL)
			printf("User %d: \t Name: %s\t Pipe: pipe_%d\n", i+1, users[i].nome, users[i].pid);
		}
	}

	
}

int main(int argc, char * const argv[], char* envp[]) {

	signal(SIGINT, sigintHandler);


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

	users = malloc(s->maxUsers * sizeof(user));
	texto = malloc(s->max_l * sizeof(singleLine));

	int j;
	for(j = 0; j < s->max_l; j++)
		texto[j].text = malloc(s->max_c * sizeof(char));
	
	for(j = 0; j < s->max_l; j++)
		strcpy(texto[j].text, "Esta linha tem quarenta caracteres e pronto  ");


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