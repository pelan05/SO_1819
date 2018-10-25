#include "clientMain.h"



int main(int argc, char *argv[]) {

	int x, y;

	initscr();//iniciar controlo do ncurses sobre a janela 'stdscr'
	scrollok(stdscr ,FALSE);//impede o auto-scroll na janela de cmd
	//getyx(stdscr, y, x);//coordenadas atuais do ponteiro


	WINDOW * linhas = newwin(15,45,0,0);	/*altura, comprimento, xinicial, yinicial*/








	endwin();//fechar ncurses control
	return EXIT_SUCCESS;

}