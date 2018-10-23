//testing all things


#include "comands.h"

int fd_global;
login_t * h = NULL;

int Separar(char * s, char * arg[], size_t t){
  char * delimitador;
  int counter = 0;

  for(int i = 0; i < t; i++){
    if(i) // if (i != 0)
      delimitador = strtok(NULL, " ");
    else
      delimitador = strtok(s, " ");

    if(!delimitador){
      arg[i] = NULL;
      break;
    }

    arg[i] = delimitador;
    counter++;
  }

  return counter;
}

void trataSig(int s){
	if (s == SIGINT)
		trataSig(SIGUSR1);
	if (s == SIGUSR1){
		printf("Shutting down!!\n");
		close(fd_global);
		unlink(SERVER_FIFO);
		
		if(logoutAll(h))
			printf("All clients have been disconnected!!\n");
		
		printf("Shutdown was successful!!\n");
		exit(0);
	}
	return;
}

void comandos(jogo * g){
	int fd, fd_cli, n, auth, argSize;
	char user[20], pass[20], cmd[50], * arg[3], * clifile_name, cli_pipe_name[50];
	login_t * curr;
	bool login = false;
  
  int nfd;
  fd_set read_fds;
  struct timeval tv;

  printf("Server PID: %d\n", getpid());
  signal(SIGUSR1,trataSig);
  signal(SIGINT, trataSig);
  
  if(!access(SERVER_FIFO, F_OK)){
	  printf("Server is already running!!!\n");
	  return;
  }
  
  if(mkfifo(SERVER_FIFO, 0600) != 0){
    perror("Couldn't create FIFO!!!\n");
    return;
  }
  
  if((fd_global = fd = open(SERVER_FIFO, O_RDWR | O_NONBLOCK)) == -1){
    perror("Couldn't open FIFO!!!\n");
    unlink(SERVER_FIFO);
    return;
  }
  
  while(1){
	tv.tv_sec = 1;
    tv.tv_usec = 0;

    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);
    FD_SET(fd, &read_fds);

    nfd = select(fd + 1, &read_fds, NULL, NULL, &tv);
	
	if(!nfd)
		continue;
	
	if(nfd == -1){
		perror("\nError in selecting!!\n");
		trataSig(SIGUSR1);
	}
	
    printf("> ");
    fgets(cmd, CMDSIZE, stdin);

    if(strlen(cmd) < 2)
      continue;
    else
      cmd[strlen(cmd) - 1] = '\0';

    argSize = Separar(cmd, arg, 3);

    if(!strcmp("add", arg[0])){
      if(argSize == 3){
	       if(newU(arg[1], arg[2], LOGIN_FILE))
	        printf("User added!\n");
	       else
	        printf("User exists already!!\n");
        }
        else
          printf("Command error. Example: add user password\n");

        continue;
    }

    if(!strcmp("users", arg[0])){
		printf("You've selected users\n");
      continue;

    if(!strcmp("players", arg[0])){
      		if(curr){
			printf("Listing clients!!\n");
			while(curr){
				printf("PID: %d, NAME: %s\n", curr->pid, curr->usr);
				curr = curr->next;
				putchar('\n');
						}
			}
			else
				printf("There are no clientes connected!!!!\n");
      continue;
    }

    if(!strcmp("kick", arg[0])){
      printf("You've select kick\n");
      continue;
    }
	
	if(!strcmp("game", arg[0])){
      printf("You've select game\n");
      continue;
    }

    if(!strcmp("play", arg[0])){
      printf("You've selected play game\n");
      continue;
    }

    if(!strcmp("shutdown", arg[0])){
      printf("You've select shuwdown game\n");
      return;
    }

    if(!strcmp("map", arg[0])){
      printf("Carrega mapa\n");
      continue;
    }

    if(!strcmp("clear", arg[0])){
      system("clear");
      continue;
    }

    if(!strcmp("help", arg[0])){
      printf("Commands available:\n add, users, players, kick, game, play, shutdown, map, clear!!\n");
      continue;
    }
	
	if(FD_ISSET(fd, &read_fds)){
		do{
			n = read(fd, &curr, sizeof(curr));
		}while(n==0);
		
	if(sscanf(curr->msg, "%s %s", user, pass) == 2)
		login = true;
	
	if(!strcmp(user, "logoff")){
	userLogout(&h, curr->pid, 0);
	printf("[Client has logged off -> %d\n", curr->pid);
	continue;
      }
	  
	  if(login){
		  sprintf(cli_pipe_name, CLIENT_FIFO, curr->pid);
		  
		  if((fd_cli = open(cli_pipe_name, O_WRONLY | O_NONBLOCK)) == -1){
			sprintf(cmd, "Couldn't open cli %d fifo", curr->pid);
			perror(cmd);
			kill(curr->pid, SIGINT);
			memset(cmd, 0, strlen(cmd));
	  continue;
	  
	  if(checkLog(h, user)){
		  auth = -1;
	  } else {
		  if(checkU(user, pass, LOGIN_FILE)){
			  h = addU(h, user, pass, curr->pid);
			  auth = 1;
		  }else
			  auth = 0;
		  
		n = write(fd_cli, &auth, sizeof(auth));
		if(n > 0){
			if(auth == 1){
				printf("[CLIENT LOGGED IN] -> %s %d\n", user, curr->pid);
			} else
				printf("[CLIENT DID NOT LOG IN] \n");
			}
			kill(curr->pid, SIGINT); 
	  
	login = false;
	close(fd_cli);
	  

    printf("Command not recognized!\n");
			}
		}
	  }
	}
	  

