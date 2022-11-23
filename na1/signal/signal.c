#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <signal.h>
#include <stdlib.h>

int final = 0;

void myfunc(int signum){ //el sigalarm necesitta una funcio void que rebi un int. signum diu quina es la senyal que m'han enviat i m'ha portat aqui
	printf("He rebut la senyal %d\n",signum);
	final = 1;
}

int main (int argc, char **argv){ //TAULA DE CANALS. Hi ha 64. S'obre seqüencialment.

	if(argc==2){ //si tiro aquest programa amb un argument, enviare un kill al programa de l'argument
		kill (atoi(argv[1]), SIGKILL); //kill no mata, dispara una senyal que puede ser sigkill u otra
		return 0;
	}
	
	
	printf("Sóc el procés %d\n",getpid());
	
	signal(SIGALRM, myfunc); //quan salti l'alarma, crida a la funcio amb el valor signalrm
	
	signal(SIGKILL, myfunc); //intento rebre la senyal sigkill amb la mateixa funció que l'alarma però aquesta senyal no es pot tractar
	
	alarm(20); //el s.o em tirara una alarma en 5 segons, que es term i moriré si no la tracto d'una altra manera (myfunc)
	
	while(!final){
		sleep(1);
		printf("*\n");
	}
	
	printf("He acabat\n");
	
}
