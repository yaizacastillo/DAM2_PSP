#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main (int argc, char **argv){
	int semid, i, val;
	pid_t pid;
	unsigned short int data[2]; //els valors que utilitzare per pasar i per llegir els valors dels semafors un cop inicialitzats
	struct sembuf sbuf[3]; //estructura ja definida per utilitzar semop

	if((semid = semget(IPC_PRIVATE, 2, IPC_CREAT))<0){//dos semafors en un mateix sector. privats. quan s'acaba deixen la memoria.
		perror("semget");
		return 1;
	}

	//inicialitzacio individual dels semafors
	//semctl(semid, 0, SETVAL, 1); //valor 1 al semafor 0. pot tenir un proces.
	//semctl(semid, 0, GETALL, data); //agafa es valors de tots i els guarda a data

	//init grupal
	data[0] = 1; data[1] = 1;
	semctl(semid, 0, SETALL, data);


	sbuf[0].sem_num = 0; sbuf[0].sem_op = -1; sbuf[0].sem_flg= 0; //resta un valor al primer semaforo. es el mateix -1 que -50, només importa positiu (sortir) o negatiu(entrar)
	sbuf[1].sem_num = 0; sbuf[1].sem_op = -1; sbuf[1].sem_flg= 0; //es queda esperant a poder entrar al primer semafor i llavors no fa la resta
	semop(semid, sbuf, 2); //2 es que faci les 2 comandes alhora, 1 faria nomes la primera

	for(i = 0; i < 2; i++){
		printf("El valor del sem %d es %d\n", i, data[i]);
	}

	//val = semctl(semid, 1, GETVAL, 0); //retorna valor d'un semafor en concret

	semctl(semid, IPC_RMID, 0);
	
	return 0;
	
}
