#include "clientMain.h"

int main(int argc, char *argv[], char* envp[]) {


	//
	//Variáveis
	//

	//int x, y;

	//int i = 0, j = 0;
	//int nrow, ncol, oposx, oposy; //num rows, num colunas, posx atual, posxy atual, old posx, old posy
	int posx, posy;
	int ch; //character da keypress em numero int (ascii)
	//char oldchar = ' ';
	//char help; //para imprimir numeros nas linhas
	
	const char *linha[NUMLINHAS] = {"Este texto e muito bonito.",
										"Este texto e mais bonito.", 
											"Este texto e ainda mais bonito!", 
												"Este texto e muito bonito.",
													"Este texto e mais bonito.", 
														"Este texto e ainda mais bonito!", 
															"Este texto e muito bonito.",
																"Este texto e mais bonito.", 
																	"Este texto e ainda mais bonito!", 
																		"Este texto e muito bonito.",
																			"Este texto e mais bonito.", 
																				"Este texto e ainda mais bonito!",
																					"Este texto e muito bonito.",
																						"Este texto e mais bonito.", 
																							"Este texto e ainda mais bonito!"};
	int choice;//vars para selecionar linha
	int highlight = 0;//1-15//a linha 1 começa selecionada

	int i=0;

	//
	//Inicio Codigo nCurses
	//


	initscr();//iniciar controlo do ncurses sobre a janela 'stdscr'
	scrollok(stdscr ,FALSE);//impede o auto-scroll na janela de cmd
	clear();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	
	
	//getyx(stdscr, y, x);//coordenadas atuais do ponteiro

	WINDOW * nomes = newwin(TAMJANY,TAMJANNOMESX,0,0);					/*altura, comprimento, yinicial, xinicial*/
	WINDOW * linhas = newwin(TAMJANY,TAMJANLINHASX,0,TAMJANNOMESX + 1);	/*altura, comprimento, yinicial, xinicial*/
	
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
	
	/*posx = 1;//posiçao relativa à janela NOMES
	posy = 1;

	wmove(linhas, posx, posy);
	refresh();*/

	while(1){
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
			default:
			break;
		}

	}





	// mexer asterisco pelo ecra - a funcionar - ~exemplo 2
	/*

	oldchar = mvwgetch(linhas, posx, posy);
	
	do{
		ch = wgetch(linhas);
		oposx = posx;
		oposy = posy;

		switch(ch){
			case KEY_UP:

				posy = (posy > 1)? posy - 1 : posy;
				break;
			case KEY_DOWN:
				posy = (posy < (TAMJANY - 2))? posy + 1: posy;
				break;
			case KEY_LEFT:
				posx = (posx > 1)? posx - 1: posx;
				break;
			case KEY_RIGHT:
				posx = (posx < (TAMJANLINHASX - 2))? posx + 1: posx;
				break;
			case KEY_ENTER:
				//TODO "Enter to edit line"
				//posy é igual ao numero da linha
				break;
		}
		if(ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT){
			
			mvwaddch(linhas, oposy, oposx, ' ');
			//oldchar = mvwgetch(linhas, posy, posx);
			//attron(COLOR_PAIR(1));
			mvwaddch(linhas, posy, posx, '*');//cursor atual
			//attroff(COLOR_PAIR(1));
			
			////mvprintw(0, 0, "(%d, %d) ", posy, posx);
			refresh();
			wrefresh(linhas);
			wrefresh(nomes);
			for(i = 1; i < (TAMJANY-2)+1; i++){
				help = i + '0'; //transforma i em char
				mvwaddch(stdscr, i, TAMJANNOMESX, help);//ecra, y, x, char
			}

		}


	}while(ch != KEY_EXIT);
	*/






	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}