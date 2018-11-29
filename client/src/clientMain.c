#include "clientMain.h"

int verificaServidor(char * path){

	if(!access( path , F_OK ) == 0){
        printf("Servidor não foi executado!\n");
		return 0;
	}else return 1;
}

int temEspaco(char * string){
	if(string[44]==' ')
		return 1;
	else 
		return 0;
}
int apagaCarat(char * string, int val){
	//preenche o slot 'val' com a celula seguinte, até chegar à 43*!!(nao puxar o \0!!!)
	for(int i = val; i < 44; i++){		
		string[i] = string[i + 1];
	}
	string[44/*strlen(string)*/] = ' ';

	return 1;
}
int adicionaCarat(char * string, char adicao, int val){

	int index = 44;//ultima celula da string

	//descobrir a começar pelo fim onde acabam as celulas vazias ' '
	//na ultima celula vazia preencher com as cheias até o slot pretendido estar vazio	
	if(temEspaco(string)){	
		for(index = TAMJANLINHASX-3; index > 0; index--){
			if(string[index] == ' ')
				continue;
			else 
				break;
		}

		index++;
		for(;index > val; index--){
			string[index] = string [index - 1];
		}
		
		string[val] = adicao;

		return 1;
	}else 
		return 0;
}


void printLinhas(WINDOW *linhas, WINDOW *nomes, char **linha, int highlight){

int i = 0;

	for(i = 0; i < NUMLINHAS; i++){

		if(i == highlight){
			wattron(linhas, A_UNDERLINE);//A_REVERSE
			mvwprintw(linhas, i+1, 1, linha[i]);
			wattroff(linhas, A_UNDERLINE);//A_REVERSE
		}else mvwprintw(linhas, i+1, 1, linha[i]);
			
		wrefresh(linhas);
		wrefresh(nomes);
		refresh();
	}

}

void getUserEnv(int argc, char * const argv[], char * username, char * path){
	int flag;

	opterr = 0;

	while((flag = getopt(argc, argv, "u:p:")) != -1)
		switch (flag) {
		case 'u':
			strcpy(username, optarg);
			break;
		case 'p':
			strcpy(path, optarg);
			break;
		case '?':
			if (optopt == 'u')
				fprintf(stderr, "Option -%c requires an argument, the UserName.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option -%c.\n", optopt);
			else
				fprintf(stderr, "Unknown option character \\x%x.\n", optopt);
		}

}

void pedeUser(char * username){
	int flagUserSuccess = 0;
	
	do{
			printf("Username: ");
			do{ 
			scanf(" %s", username);
			}while(strlen(username) > 8 || strlen(username) < 1);
			//TODO checkar user no server
			//flagUserSuccess = check();

			flagUserSuccess = 1;//para testes

	}while(!flagUserSuccess);
}

void wPrintNumbers(WINDOW *numeros){

	char help; //para imprimir numeros nas linhas
	int i = 0, j = 10;

	for(i = 0; i < 10; i++){//01-09
		help = (char ) i+'0';
		mvwaddch(numeros, i, 0, '0');
		mvwaddch(numeros, i, 1, help);
	}
	for(i = 0; i < 5 ; i++){//10-14
		help = (char ) i+'0';
		mvwaddch(numeros, j, 0, '1');
		mvwaddch(numeros, j, 1, help);
		j++;
	}
	wrefresh(numeros);
}
/*
//TODO USERNAME srvr verification
int usernameExists(char * username){

//ON Success 
return 1;

//else return 0;


}
*/

/*
char * leFifo(char * path, char * String){
	
	int fd = open( path , O_RDONLY );
	if(fd == -1)
		printf("Erro a abrir o fifo %s \n", path);

	read(fd, String, sizeof(String));
	read(fd, pid, sizeof(int));

	close(fd);
	return String;
}
*/
void olaFifo(char * path, char * String, int pid){
	
	int fd = open( path , O_WRONLY );
	if(fd == -1)
		printf("Erro a abrir o fifo %s \n", path);
	
	write(fd, String, sizeof(String));
	write(fd, pid, sizeof(int));

	close(fd);
}




///----------------------------------------
///MAIN
///
///
///
///----------------------------------------




int main(int argc, char * const argv[]) {


	//
	//Variáveis
	//



	char username[8] = {" "};
	char path[50] = {"../../server/bin/"};
	char file[20] = {"serverPipe"};

	char **linha;
	
	linha = malloc(sizeof(char *) * 15);
	if(!linha){
		printf("Erro a alocar mem linha 191 clientmain\n");
	}
	for(int i = 0; i < 15; i++)
	{
		linha[i] = malloc(sizeof(char) * 45);
		if(!linha[i])
		{
			printf("Erro a alocar mem linha 198 clientmain\n");
		}
	}
	

		strcpy(linha[0], "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(linha[1], "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(linha[2], "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(linha[3], "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(linha[4], "Esta linha tem quarenta caracteres e pronto  ");
		strcpy(linha[5], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[6], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[7], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[8], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[9], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[10], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[11], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[12], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[13], "Esta linha tem quarenta caracteres e pronto k");
		strcpy(linha[14], "Esta linha tem quarenta caracteres e pronto k");


	int choice;//vars para selecionar linha
	int highlight = 0;//1-15//a linha 1 começa selecionada
	
	//int i=0;

	//
	//Codigo do username
	//
	
	getUserEnv(argc, argv, username, file);//-u "nome" e -p "path do main pipe"

	strcpy(path, strcat(path, file));//compila string de path e verifica se pip existe
	if(!verificaServidor(path)){
		exit(1);
	}
	if(!strcmp(username, " ")){//pede por linha de comandos
		pedeUser(username);
	}

	//TODO verificaçã
	olaFifo(path, username, getpid());



	/*
	if(!usernameExists(username)){
		printf("Username não encontrado!");
		exit(1);
	}
	*/




	//
	//Inicio Codigo nCurses
	//

	initscr();//iniciar controlo do ncurses sobre a janela 'stdscr'
	scrollok(stdscr ,FALSE);//impede o auto-scroll na janela de cmd
	clear();
	start_color();
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	
	
	//getyx(stdscr, y, x);//coordenadas atuais do ponteiro

	WINDOW * nomes = newwin(TAMJANY,TAMJANNOMESX,0,0);					/*altura, comprimento, yinicial, xinicial*/
	WINDOW * linhas = newwin(TAMJANY,TAMJANLINHASX,0,TAMJANNOMESX + 2);	/*altura, comprimento, yinicial, xinicial*/
	WINDOW * numeros = newwin(15,2,1,TAMJANNOMESX);	/*altura, comprimento, yinicial, xinicial*/
	
	box(nomes, '|' , '-'); //janela, vert char, horiz char
	box(linhas, '|' , '-'); //janela, vert char, horiz char

	//wRefreshAll(nomes, numeros, linhas); //not working
	refresh();

	curs_set(0);//cursor 0-invisivel, 1-normal, 2-high-vis mode
	
	noecho();
	cbreak();
	keypad(linhas, TRUE);
	refresh();
	wrefresh(linhas);
	wrefresh(nomes);
	
	//imprimir numeros
	wPrintNumbers(numeros);		





	do{

		printLinhas(linhas, nomes, linha, highlight); //works 5*

		choice = wgetch(linhas);
		
		switch(choice){
			case KEY_UP:
				highlight = (highlight > 0)? highlight-1: highlight;
				break;
			case KEY_DOWN:
				highlight = (highlight < (NUMLINHAS-1))? highlight+1: highlight;
				break;
			case 10://enter
				mvwprintw(nomes, highlight+1, 1, username);
				wrefresh(nomes);

				//TODO cursor no canto em editmode, delete é considerado 'default' escreve um 'J'
					curs_set(1);
					
					//wmove(linhas, highlight + 1, 1);//window, y, x
					wmove(linhas, 5, 5);
					wrefresh(linhas);
					refresh();

					int choice;
					char ch;
					int cursor = 0;

					do{	
						choice = '0';
						printLinhas(linhas, nomes, linha, highlight);	
						choice = wgetch(linhas);

							switch(choice){
								case KEY_LEFT:
									cursor = (cursor < 2)? cursor : cursor - 1;
									break;
								case KEY_RIGHT:
									cursor = (cursor < (TAMJANLINHASX-2))? cursor + 1 : cursor;
									break;
								case 8://backspace(8)
									apagaCarat(linha[highlight], cursor-2);
									cursor--;
									//cursor = (cursor < 2)? cursor : cursor--;
									break;
								case 127://delete(127)//o backspace está a mandar 127 nesta versao do linux (Pedro - Manjaro);
									apagaCarat(linha[highlight], cursor-2);
									cursor--;
									break;
								case KEY_DL://delete(127)//----------------//TODO delete nao funciona na minha versao -pedro
									apagaCarat(linha[highlight], cursor-1);
									break;
								case 10: //enter
									break;
								case 27: //escape
									break;
								case KEY_UP://para nao irem ambos para 'default'
									break;
								case KEY_DOWN:
									break;
								default: //character para escrever
									ch = choice;
									if(adicionaCarat(linha[highlight], ch, cursor-1))
										cursor++;
									break;
							}
					wmove(linhas, highlight + 1, cursor);
					}while(choice != 10 && choice != 27);//enter ou escape

					curs_set(0);
					

				mvwprintw(nomes, highlight+1, 1, "        ");
				wrefresh(nomes);

				//printLinhas(linhas, nomes, linha, highlight);

				break;
			default:
				break;
		}

	}while(choice != 27);//esc


	//
	//endGame
	//
	free(linha);
	endwin();//fechar ncurses control
	

	
	return EXIT_SUCCESS;

}