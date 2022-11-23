#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char **argv){ //TAULA DE CANALS. Hi ha 64. S'obre seqüencialment.
	int fd, pid = fork();
	
	if(pid == 0){
		//TODO ESTO SOLO AFECTA AL HIJO. EL PADRE TIENE SUS FDS
		//0 teclat 1 pantalla 2 pantalla
		
		//close(1); //como cierro el 1, no me saldra por pantalla e ira al archivo que tomara este fd
		fd = open (argv[1], O_WRONLY | O_CREAT | O_TRUNC); //el fd es la posicion vacia mas baja. si cerramos el 1, el 1. si no, el 3.
		
		//0 teclat 1 pantalla 2 pantalla 3 fitxer
		
		close(1);
		
		//0 teclat 1 ........ 2 pantalla 3 fitxer
		
		dup (fd); //duplica el fd 3 al mes baix buit = 1
		
		//0 teclat 1 fitxer 2 pantalla 3 fitxer
		
		close(fd);
		
		//0 teclat 1 fitxer 2 pantalla 3 ........
		
		printf("Posició fd: %d\n",fd);
		execlp("ls", "ls", NULL);
		//me voy a hacer el ls y muero, asi que yo no puedo ahcer un printf luego. tiene que hacerlo un hijo
		perror("execlp");
		return 1;
	} else {
		wait(NULL);
		printf("HOLA");
	}
}
