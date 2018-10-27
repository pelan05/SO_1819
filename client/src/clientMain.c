#include "clientMain.h"





int main(int argc, char *argv[], char* envp[]) {

	int x, y;

	int i = 0, j = 0;
	int nrow, ncol, posx, posy, oposx, oposy; //num rows, num colunas, posx atual, posxy atual, old posx, old posy
	int ch; //character da keypress em numero int (ascii?)
	char oldchar = ' ';

	initscr();//iniciar controlo do ncurses sobre a janela 'stdscr'
	scrollok(stdscr ,FALSE);//impede o auto-scroll na janela de cmd
	clear();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	
	
	//getyx(stdscr, y, x);//coordenadas atuais do ponteiro

	WINDOW * nomes = newwin(TAMJANY,TAMJANNOMESX,0,1);		/*altura, comprimento, yinicial, xinicial*/
	WINDOW * linhas = newwin(TAMJANY,TAMJANLINHASX,0,TAMJANNOMESX + 1);	/*altura, comprimento, yinicial, xinicial*/
	
	box(nomes, '|' , '-'); //janela, vert char, horiz char
	box(linhas, '|' , '-'); //janela, vert char, horiz char

	refresh();


	noecho();
	//cbreak();
	keypad(linhas, TRUE);
	refresh();
	
	//TODO fix neste bug, nao mostra as janelas se nao puxar o wgetch
	wgetch(nomes);
	wgetch(linhas);

	posx = TAMJANNOMESX + 2;
	posy = 1;

	wmove(linhas, posx, posy);
	refresh();
	oldchar = wgetch(linhas);
	

	do{
		ch = getch();
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
				posx = (posx > (TAMJANNOMESX + 2))? posx - 1: posx;
				break;
			case KEY_RIGHT:
				posx = (posx < (TAMJANLINHASX + TAMJANNOMESX - 1))? posx + 1: posx;
				break;
			case KEY_ENTER:
				//TODO mudar para "Enter to edit line"
	
				break;
		}
		if(ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT){
			
			mvaddch(oposy, oposx, ' ');
			oldchar = mvgetch(posy, posx);
			//attron(COLOR_PAIR(1));
			mvaddch(posy, posx, '*');/*cursor atual*/
			//attroff(COLOR_PAIR(1));
			
			////mvprintw(0, 0, "(%d, %d) ", posy, posx);
			refresh();

		}


	}while(ch != KEY_EXIT);


	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}