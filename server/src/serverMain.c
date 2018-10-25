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


int main(int argc, char const *argv[]){

    





    return 0;
}
