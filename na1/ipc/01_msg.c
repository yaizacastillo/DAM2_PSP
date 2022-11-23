#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct{ //sobre
	long mtype;
	int a;
	int b;
}t_data;

int main (int argc, char **argv){ //suma dos numeros per msg
	int msgid;
	pid_t pid;
	t_data data;

	if((msgid = msgget(IPC_PRIVATE, IPC_CREAT))<0) {//retorna indentificador per una bústia compartida amb mi mateix (i fills). quan el proces mori, com es privat, la memoria s'allibera
		perror("msgget");
		return 1;
	}

	if((pid = fork())<0){  //el hijo tambien tendra la direccion de la bustia
		perror ("fork");
		return 1;
	}

	if(pid==0){ //soc el fill. vull que tiri el sobre al pare.
		sleep(2);
		data.mtype = 1; //si no l'inicialitzem, hi haura basura. si la basura es negativa, no funciona.
		data.a = 3;
		data.b = 5;

		msgsnd(msgid, &data, sizeof(data) - sizeof(long), 0); //0 comportament per defecte: si no, m'espero
		return 0;
	}

	wait(NULL); //espero la mort d'un fill. la carta ha arribat si o si. si no ho poso, em bloquejare fins que arribi per msgrcv.

	msgrcv(msgid, &data, sizeof(data) - sizeof(long), 0, 0); //en aquest punter no hi ha res ara. el fill ha omplert el seu. el 0 es el message type que estic esperant. rep la primera carta que troba. la prioritat negativa fa que rebi cartes del 0 al seu valor absolut. 0 per fer-ho de forma bloquejant a que la carta hi sigui

	printf("La suma de %d i %d es %d\n", data.a, data.b, data.a + data.b);	
	
	
}
