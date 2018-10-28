#include "clientMain.h"

int temEspaco(char * string){
	if(string[43]==' ')
		return 1;
	else return 0;
}
int apagaCarat(char * string, int val){
	int i = 0;
	//preenche o slot 'val' com a celula seguinto, até chegar à 43*!!(nao puxar o \0!!!)
	for(i = val; i < 44; i++){
		string[i] = string[i+1];
	}
	string[43] = ' ';
	return 1;
}
int adicionaCarat(char * string, char adicao, int val){
	int index = 43;
	if(temEspaco(string)){
	//descobrir a começar pelo fim onde acabam as celulas vazias ' '
	//na ultima celula vazia preencher com as cheias até o slot pretendido estar vazio		
		for(index = TAMJANLINHASX-3; index > 0; index--){
			if(string[index] == ' ')
			continue;
			else break;
		}
		for(;index > val; index--){
			string[index] = string [index -1];
		}
		string[val] = adicao;

		return 1;
	}else return 0;
}

void editMode(char * string, WINDOW ** window, int linha){

curs_set(1);
wmove(*window, linha, 1);//window, y, x
int choice;
char ch;
int cursor = 1;

do{
		

		choice = wgetch(*window);

		switch(choice){
			case KEY_LEFT:
			cursor = (cursor < 2)? cursor: cursor -1;
			break;
			case KEY_RIGHT:
			cursor = (cursor < (TAMJANLINHASX-2))? cursor +1: cursor;
			break;
			//TODO edit mode here
			//------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------
			case 8://backspace
			apagaCarat(string, cursor);			

			break;
			case 127://delete
			apagaCarat(string, cursor+1);
			
			break;
			default:
			ch = choice;
			adicionaCarat(string, ch, cursor);


			break;
		}

wmove(*window, linha, cursor);

}while(choice != 10);//enter

curs_set(0);

}

void getUserEnv(int argc, char const *argv[], char* envp[], char * username){

	char* aux = NULL;

	int flag;

	opterr = 0;

	while((flag = getopt(argc, argv, "u:")) != -1)
		switch (flag) {
		case 'u':
			strcpy(username, optarg);
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




int main(int argc, char *argv[], char* envp[]) {


	//
	//Variáveis
	//

	int posx, posy;
	char * help; //para imprimir numeros nas linhas
	char username[8] = {" "};



	const char *linha[NUMLINHAS] = {"Texto decente.",
									"Mais texto decente.", 
									"Muito mais texto decente", 
									"Mais texto decente..",
									"Mais texto decente.", 
									"Mais texto decente.", 
									"Mais texto decente.",
									"Mais texto decente.", 
									"Mais texto decente.", 
									"Mais texto decente.",
									"UwU what's this?", 
									"Anticonstitucionalissimamente.Anticonstitucio",
									"Anticonstitucionalissimamente.Anticonstitucio",
									"Anticonstitucionalissimamente.Anticonstitucio", 
									"Anticonstitucionalissimamente.Anticonstitucio"};
	int choice;//vars para selecionar linha
	int highlight = 0;//1-15//a linha 1 começa selecionada
	
	int i=0;

	//
	//Codigo do username
	//
	
	getUserEnv(argc, argv, envp, username);
	if(!strcmp(username, " ")){
		pedeUser(username);
	}



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

	

	refresh();

	curs_set(0);//cursor 0-invisivel, 1-normal, 2-high-vis mode
	
	noecho();
	cbreak();
	keypad(linhas, TRUE);
	refresh();
	wrefresh(linhas);
	wrefresh(nomes);
	
					//imprimir numeros//not the prettiest solution but t'works
					for(i = 0; i < 10; i++){
						help = i+'0';
						mvwaddch(numeros, i, 1, help);
					}
					for(i = 0; i < 10 ; i++){
						help = 0+'0';
						mvwaddch(numeros, i, 0, help);
					}
						help = 0+'0';
						mvwaddch(numeros, 10, 1, help);
						help = 1+'0';
						mvwaddch(numeros, 10, 0, help);

						help = 1+'0';
						mvwaddch(numeros, 11, 1, help);
						help = 1+'0';
						mvwaddch(numeros, 11, 0, help);

						help = 2+'0';
						mvwaddch(numeros, 12, 1, help);
						help = 1+'0';
						mvwaddch(numeros, 12, 0, help);

						help = 3+'0';
						mvwaddch(numeros, 13, 1, help);
						help = 1+'0';
						mvwaddch(numeros, 13, 0, help);

						help = 4+'0';
						mvwaddch(numeros, 14, 1, help);
						help = 1+'0';
						mvwaddch(numeros, 14, 0, help);

					wrefresh(numeros);




	do{
		for(i = 0; i < NUMLINHAS; i++){
			

			if(i == highlight){
				wattron(linhas, A_REVERSE);
				mvwprintw(linhas, i+1, 1, linha[i]);
				wattroff(linhas, A_REVERSE);
			}else
			
			mvwprintw(linhas, i+1, 1, linha[i]);

			
			wrefresh(linhas);
			wrefresh(nomes);
			refresh();
		}

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
			editMode(linha[highlight], &linhas, highlight+1);//frase, janela e num linha
			mvwprintw(nomes, highlight+1, 1, "        ");
			wrefresh(nomes);

			for(i = 0; i < NUMLINHAS; i++){
			

			if(i == highlight){
				wattron(linhas, A_REVERSE);
				mvwprintw(linhas, i+1, 1, linha[i]);
				wattroff(linhas, A_REVERSE);
			}else
			
			mvwprintw(linhas, i+1, 1, linha[i]);

			
			wrefresh(linhas);
			wrefresh(nomes);
			refresh();
			}


			break;
			default:
			break;
		}

	}while(choice != 27);//esc


	//
	//endGame
	//

	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}