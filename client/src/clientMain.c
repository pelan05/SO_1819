#include "clientMain.h"

#define TAMJANLINHASX 45
#define TAMJANY 17
#define TAMJANNOMESX 10




int main(int argc, char *argv[], char* envp[]) {

	int x, y;

	int nrow, ncol, posx, posy, oposx, oposy; //num rows, num colunas, posx atual, posxy atual, old posx, old posy
	int ch; //character da keypress em numero int (ascii?)

	initscr();//iniciar controlo do ncurses sobre a janela 'stdscr'
	scrollok(stdscr ,FALSE);//impede o auto-scroll na janela de cmd
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	
	
	
	//getyx(stdscr, y, x);//coordenadas atuais do ponteiro

	WINDOW * nomes = newwin(TAMJANY,TAMJANNOMESX,0,1);		/*altura, comprimento, yinicial, xinicial*/
	WINDOW * linhas = newwin(TAMJANY,TAMJANLINHASX,0,TAMJANNOMESX + 1);	/*altura, comprimento, yinicial, xinicial*/
	
	box(nomes, '|' , '-'); //janela, vert char, horiz char
	box(linhas, '|' , '-'); //janela, vert char, horiz char

	refresh();
	
	//TODO fix neste bug, nao mostra as janelas se nao puxar o wgetch
	wgetch(nomes);
	wgetch(linhas);

	posx = TAMJANNOMESX + 2;
	posy = 1;

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
		}
		if(ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT){
			/*cursor anterior*/mvaddch(oposy, oposx, ' ');
			/*cursor atual*/mvaddch(posy, posx, '*');
			mvprintw(0, 0, "(%d, %d) ", posy, posx);
			refresh();

		}



	}while(ch != KEY_BACKSPACE);


	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}