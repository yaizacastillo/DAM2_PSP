#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

int main (int argc, char **argv) {
	int semid;
	int shmid;
	
	pid_t pid;
	int i, *val;
	unsigned short int data [2];
	
	struct sembuf sbuf[3];
	
	if((semid = semget(IPC_PRIVATE, 2, IPC_CREAT))<0){
		perror("semget");
		return 1;
	}
	
	if((shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT))<0){
		perror("shmget");
		return 1;
	}
	
	data[0] = 0; data[1] = 0;
	semctl(semid, 0, SETALL, data);
	
	if((pid = fork())<0){
		perror("fork");
		return 1;
	}
	
	if(pid==0){
		sbuf[0].sem_num = 0; sbuf[0].sem_op = 1; sbuf[0].sem_flg = 0;
		semop(semid, sbuf, 1);
		
		sbuf[1].sem_num = 1; sbuf[1].sem_op = -1; sbuf[1].sem_flg = 0;
		semop(semid, &(sbuf[1]), 1);
		printf("Estic desbloquejat i marxo\n");
	}
	
	wait(NULL);
	semctl(semid, IPC_RMID, 0);
	
	return 0;
}
