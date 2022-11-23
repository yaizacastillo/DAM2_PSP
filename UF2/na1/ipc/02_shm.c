#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct{
	int a;
	int b;
}t_data;

int main (int argc, char **argv){ //suma dos numeros per msg
	int shmid;
	pid_t pid;
	t_data *data;

	if((shmid = shmget(IPC_PRIVATE, sizeof(t_data), IPC_CREAT))<0) {
		perror("shmget");
		return 1;
	}
	
	data = (t_data *) shmat(shmid, 0, 0); //de momento se hace asi y. //los 8 bytes que te da interpretalos como un puntero a t_data. data apunta a la memoria compartida.

	if((pid = fork())<0){ 
		perror ("fork");
		return 1;
	}

	if(pid==0){ //soc el fill. vull que tiri el sobre al pare.
		sleep(2);
		data->a = 3; //porque es un puntero
		data->b = 5;

		return 0;
	}

	wait(NULL);

	printf("La suma de %d i %d es %d\n", data->a, data->b, data->a + data->b);
	
	shmctl(shmid, IPC_RMID, NULL);//en morir el pare quedaria lliberada. Null perque depenent del segon parametre podria necessitar un tercer
	
}
