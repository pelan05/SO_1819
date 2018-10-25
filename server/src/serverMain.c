#include "serverMain.h"

int findUser(char* user, char* filename) {


	char buffer[TAMBUFFER];

	File f = fopen(filename, "r");
	if (f == NULL) {
		printf("Erro a abrir Ficheiro");
		return 2;
	}

	while (fgts(buffer, sizeof(buffer), f) != NULL)
		if (!str(user, buffer))
			return 1;

	return 0;
}

void commands() {

	/*
	char cmd[50], *arg[3];
	int argSize;
	*/


	if (!strcmp("settings", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("load", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("save", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("free", arg[0])) {
		printf("You've select free\n");
		continue;
	}

	if (!strcmp("statistics", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("users", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("text", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("shutdown", arg[0])) {
		printf("You've select Settings\n");
		continue;
	}

	if (!strcmp("help", arg[0])) {
		printf("You've selected help:\n");
		printf("'settings' will display the current system settings.\n");
		printf("'load <filename>' will load the content of <filename>.\n");
		printf("'save <filename>' will save what's being editted in <filename>.\n");
		printf("'free <linenumber>' erase contents of <linenumber>.\n");
		printf("'statistics' will display some statistics by the second while waiting for another command.\n");
		printf("'users' will a list of what users are logged in, by order of arrival.\n");
		printf("'text' will the current text on client side.\n");
		printf("'shutdown' will quit without saving.\n");
		continue;
	}


}


int main(int argc, char const *argv[], char* envp[]){

    





    return 0;
}
