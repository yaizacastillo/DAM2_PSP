#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){

	char b[512];
	int pid;
	int ppid;
	int ret;
	int status;
	
	if((ret = fork ()) == -1){ //crea un proceso hijo. devuelve el pid del hijo al padre, 0 al pid del hijo. si falla devuelve -1 y el hijo no se crea.
		perror("fork");
		return 1;
	}
	
	pid = getpid();
	ppid = getppid();
	
	//ps -ef enseña todos los procesos del sistema
	//kill -9 pid para matar un proceso
	
	if(ret == 0){ //soc el fill
		sleep(1); //se espera 1 segundo
		sprintf(b, "FILL. Sóc el procés %d i el meu pare és el %d. El resultat del fork és: %d\n", pid, ppid, ret); //escribe en el buff (b)
		write(1, b, strlen(b));
		return 3;
	}
	
	else {
		sprintf(b, "PARE. Sóc el procés %d i el meu pare és el %d. El resultat del fork és: %d\n", pid, ppid, ret);
		write(1, b, strlen(b));
		
		wait(&status); //espera a que el hijo termine y recoge la informació de cómo ha ido su proceso
		printf("Final pare: %d\n", WEXITSTATUS(status));
	}
	
	return 0;
}
