#include "clientMain.h"

int main(int argc, char *argv[], char* envp[]) {

	int x, y;

	int i = 0, j = 0;
	int nrow, ncol, posx, posy, oposx, oposy; //num rows, num colunas, posx atual, posxy atual, old posx, old posy
	int ch; //character da keypress em numero int (ascii)
	char oldchar = ' ';
	char help; //para imprimir numeros nas linhas

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

	curs_set(1);//cursor 0-invisivel, 1-normal, 2-high-vis mode
	
	noecho();
	cbreak();
	keypad(linhas, TRUE);
	refresh();
	wrefresh(linhas);
	wrefresh(nomes);
	
	posx = 1;//posiçao relativa à janela NOMES
	posy = 1;

	wmove(linhas, posx, posy);
	refresh();
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
			mvwaddch(linhas, posy, posx, '*');/*cursor atual*/
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


	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}