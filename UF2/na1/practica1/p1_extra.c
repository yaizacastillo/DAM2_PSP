#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>

typedef struct{
	long mtype;
	char* comanda;
	int resultat;
}t_msg;

typedef struct{
	int numcomandes;
	int numcorrectes;
}t_shm;

int main (int argc, char **argv){

	//------------------PREPARAR IPC--------------------------

	int msgid, shmid, n;
	t_msg msg;
	t_shm *shm;
	
	if((msgid = msgget(IPC_PRIVATE, IPC_CREAT))<0){
		perror("msgget");
		return 1;
	}
	
	msg.mtype = 1;
	msg.comanda = (char *)malloc(strlen(argv[1])+1);
	strcpy(msg.comanda, argv[1]);
	
	if((shmid = shmget(IPC_PRIVATE, sizeof(t_shm), IPC_CREAT))<0){
		perror("shmget");
		return 1;
	}
	
	shm = (t_shm *) shmat(shmid, 0, 0);
	
	shm->numcomandes = 0;
	shm->numcorrectes = 0;
	
	//-----------------------DISTINGIR ARGUMENTS------------------------
	int MAX = 15;
	char *params [MAX];
	char inparam [MAX];
	char outparam [MAX];
	
	/*int MAX = 15;
	char *params [MAX];
	int i = 0;
	int paramsc;
	char inparam [MAX];
	char outparam [MAX];
	int isParam = 1, isIn = 0, isAppend = 0, isOut=0;
	
	printf("PID procés: %d\n", getpid());
	
	strcpy(inparam, "");
	strcpy(outparam, "");

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
		
		else if(strcmp(token, ">>") == 0){
			isOut = 1;
			isAppend = 1;
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
	printf("Paràmetre de sortida: %s, %i\n", outparam, isAppend);
	
	
	//-----------------------EXEC ARG-------------------------
	int fd, pid[3], died, status;

	if((pid[0] = fork())<0){
		perror("fork1");
		return 1;
	}
	
	if(pid[0] == 0){
		printf("Sóc el proces %d i fare l'execvp\n", getpid());
		
		//Entra per fitxer si ho demana la comanda
		if(strcmp(inparam, "")!=0){
			fd = open(inparam, O_WRONLY);
			
			close(0);
			dup(fd);
			close(fd);
		}
		
		//Surt per fitxer si ho demana la comanda
		if(strcmp(outparam, "")!=0){
			if(isAppend == 1)
			{
				fd = open(outparam, O_WRONLY | O_CREAT | O_APPEND);
			}else
			{
				fd = open(outparam, O_WRONLY | O_CREAT | O_TRUNC);
			}
			
			close(1);
			dup(fd);
			close(fd);
		}
		
		//Execució
		n = shm->numcomandes;
		n = n+1;
		shm->numcomandes = n;
		execvp(params[0], params);
		perror ("execvp 1");
      		return 1;
	}
	
	died = wait (&status);
	msg.resultat = status;
	if(status == 0){
		n = shm->numcorrectes;
		n = n+1;
		shm->numcorrectes = n;
	}
	
	msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
	
	if((pid[1] = fork())<0){
		perror("fork2");
		return 1;
	}
	
	char b[512];
	
	if(pid[1] == 0){
		printf("Soc el fill 2 i escriure el log\n");
		
		msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 0, 0);
		
		fd = open("p1.log", O_WRONLY | O_CREAT | O_TRUNC);
		
		sprintf(b, "Comanda: %s. Resultat %d\n", msg.comanda, msg.resultat);
		
		write(fd, b, strlen(b));
		return 1;
	}
	
	died = wait (&status);
	
	if((pid[2] = fork())<0){
		perror("fork3");
		return 1;
	}
	
	if(pid[2] == 0){
		printf("Soc el fill 3 i consultaré una memòria compartida\n");
		
		printf("S'han realitzat %d comandes, de les quals %d amb resultat correcte\n", shm->numcomandes, shm->numcorrectes);
		return 1;
	}
	
	died = wait (&status);
   	printf ("Ha finalitzat el proces %d amb codi %d\n", died, WEXITSTATUS (status));*/
}
