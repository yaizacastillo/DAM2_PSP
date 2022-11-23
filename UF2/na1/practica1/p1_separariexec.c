#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main (int argc, char **argv){

	//-----------------------DISTINGIR ARGUMENTS------------------------
	int MAX = 15;
	char *params [MAX];
	int i = 0;
	int paramsc;
	char inparam [MAX];
	char outparam [MAX];
	int isParam = 1, isIn = 0, isOut=0;

	char* token = strtok(argv[1], " ");
	
	while(token!=NULL && i < (MAX-1)){
		
		if(strcmp(token, "<") == 0){
			isIn = 1;
			isParam = 0;
		}
		
		else if(isIn == 1){
			strcpy(inparam, token);
			isIn = 0;
		}
		
		else if(strcmp(token, ">") == 0){
			isOut = 1;
			isParam = 0;
		}
		
		else if(isOut == 1){
			strcpy(outparam, token);
			isOut = 0;
		}
		
		else if(isParam == 1){
			params[i] = (char *)malloc(strlen(token)+1);
			strcpy(params[i], token);
			i = i+1;
		}
		token = strtok(NULL, " ");
	}
	
	paramsc = i;
	params[i] = NULL;
	paramsc = i + 1;
	
	printf("Arguments:\n");
	
	for(i = 0; i < paramsc; i++){
		printf("%i - %s\n", i, params[i]);
	}
	
	printf("Paràmetre d'entrada: %s\n", inparam);
	printf("Paràmetre de sortida: %s\n", outparam);
	
	
	//-----------------------EXEC ARG-------------------------
	int pid, died, status;

	if((pid = fork())<0){
		perror("fork1");
		return 1;
	}
	
	if(pid == 0){
		printf("Sóc el proces %d i fare l'execvp", getpid());
		execvp(params[0], params);
		perror ("execvp 1");
      		return 1;
	}

	died = wait (&status);
   	printf ("Ha finalitzat el proces %d amb codi %d", died, WEXITSTATUS (status));
}
