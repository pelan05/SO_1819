#include "clientMain.h"

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
	string[45/*strlen(string)*/] = ' ';

	return 1;
}
int adicionaCarat(char * string, char adicao, int val){
	int index = 44;
	if(temEspaco(string)){
	//descobrir a começar pelo fim onde acabam as celulas vazias ' '
	//na ultima celula vazia preencher com as cheias até o slot pretendido estar vazio		
		for(index = TAMJANLINHASX-3; index > 0; index--){
			if(string[index] == ' ')
				continue;
			else 
				break;
		}
		for(;index > val; index--){
			string[index] = string [index -1];
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
			wattron(linhas, A_REVERSE);
			mvwprintw(linhas, i+1, 1, linha[i]);
			wattroff(linhas, A_REVERSE);
		}else mvwprintw(linhas, i+1, 1, linha[i]);
			
		wrefresh(linhas);
		wrefresh(nomes);
		refresh();
	}

}
/*
void editMode(char * string, WINDOW ** window, int linha){ //frase, janela e num. linha

curs_set(1);
wmove(*window, linha, 1);//window, y, x
int choice;
char ch;
int cursor = 0;

do{		choice = wgetch(*window);

		switch(choice){
			case KEY_LEFT:
				cursor = (cursor < 2)? cursor : cursor - 1;
				break;
			case KEY_RIGHT:
				cursor = (cursor < (TAMJANLINHASX-2))? cursor + 1 : cursor;
				break;
			case 8://backspace(8)
				apagaCarat(string, cursor-3);


				break;
			case 127://delete(127)
				apagaCarat(string, cursor-2);
				break;
			case 10: //enter
				break;
			case 27: //escape
				break;
			default: //character para escrever
				ch = choice;
				adicionaCarat(string, ch, cursor-1);
				break;
		}
wmove(*window, linha, cursor);
}while(choice != 10 && choice != 27);//enter ou escape

curs_set(0);
}*/

void getUserEnv(int argc, char * const argv[], char * username){
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

void wPrintNumbers(WINDOW *numeros){

	char help; //para imprimir numeros nas linhas
	int i = 0, j = 10;

	for(i = 0; i < 10; i++){//1-9
		help = (char ) i+'0';
		mvwaddch(numeros, i, 1, help);
	}

	for(i = 0; i < 10 ; i++){//'0' à esq dos numeros
		mvwaddch(numeros, i, 0, '0');
	}

	for(i = 0; i < 5 ; i++){//10-14
		help = (char ) i+'0';
		mvwaddch(numeros, j, 0, '1');
		mvwaddch(numeros, j, 1, help);
		j++;
	}
	wrefresh(numeros);
}



int main(int argc, char * const argv[]) {


	//
	//Variáveis
	//

	char username[8] = {" "};

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
	
	getUserEnv(argc, argv, username);//-u "nome"
	if(!strcmp(username, " ")){//pede por linha de comandos
		pedeUser(username);
	}
	//TODO verificação username do srv



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
	
	//imprimir numeros//not the prettiest solution but t'works
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

				//editMode(linha[highlight], &linhas, highlight + 1);//frase, janela e num linha
				//inicio editmode
					curs_set(1);
					
					wmove(linhas, highlight + 1, 1);//window, y, x
					int choice;
					char ch;
					int cursor = 0;

					do{	
						
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
									apagaCarat(linha[highlight], cursor-3);
									break;
								case 127://delete(127)
									apagaCarat(linha[highlight], cursor-2);
									break;
								case KEY_BACKSPACE://backspace(8)//---------
									apagaCarat(linha[highlight], cursor-3);
									break;
								case KEY_DL://delete(127)//-----------------
									apagaCarat(linha[highlight], cursor-2);
									break;
								case KEY_SDL://delete(127)//----------------
									apagaCarat(linha[highlight], cursor-2);
									break;
								case 10: //enter
									break;
								case 27: //escape
									break;
								default: //character para escrever
									ch = choice;
									adicionaCarat(linha[highlight], ch, cursor-1);
									break;
							}
					wmove(linhas, highlight + 1, cursor);
					}while(choice != 10 && choice != 27);//enter ou escape

					//curs_set(0);
				//fim editmode
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

	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}