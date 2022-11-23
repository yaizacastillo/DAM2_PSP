#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

// ls - la | wc -l > out.dat
//el pare ha de crear el pipe perquè els fills l'heredin i es comuniquin entre ells i tindrà 5 canals oberts. p0 llegeix i p1 escriu. la taula del fill 1era 0 stdin 1 p1 2 stderr. la taula del f2 sera 0 p0 1 file on escriu 2 stderr

int main (int argc, char **argv){
	int pid[2], fd, p[2], err, died, status;
	
	if((err = pipe(p)) < 0){
		perror("pipe");
		return 1;
	}
	
	if((pid[0] = fork()) < 0){
		perror("fork 1");
		return 1;
	}
	
	if(pid[0] == 0){
		//Primer fill: ls
		printf("Sóc el procés %d i executaré un ls \n", getpid());
		close(1);
		dup(p[1]);	//Tinc una còpia de p[1] a la posició
		close(p[0]);	//Mai llegiré de la pipe
		close(p[1]);	//Ja el tinc a la posició 1
		
		execlp("ls", "ls", "-la", NULL); //NULL es per senyalar que s'ha acabat la llista d'arguments
		perror("execlp ls");
		
		return 1;
	}
	
	//Aquí ja sóc el pare perquè el fill mai arribarà viu aquí
	
	if((pid[1] = fork()) < 0){
		perror("fork 2");
		return 1;
	}
	
	if(pid[1] == 0){
		//Segon fill: wc -l
		printf("Sóc el procés %d i executaré un wc \n", getpid());
		close(0);
		dup(p[0]);
		close(p[0]);
		
		close(1);
		close(p[1]);
		open ("out.dat", O_WRONLY | O_CREAT | O_TRUNC, 0777); //permisos
		
		execlp("wc", "wc", "-l", NULL);
		perror("execlp wc");
		
		return 1;
	}
	
	close(p[0]);
	close(p[1]);
	
	died = wait(&status); //wait retorna el pid
	printf("Ha finalitzat el procés %d amb codi %d\n", died, WEXITSTATUS(status));
	
	died = wait(&status); //wait retorna el pid
	printf("Ha finalitzat el procés %d amb codi %d\n", died, WEXITSTATUS(status));
}
