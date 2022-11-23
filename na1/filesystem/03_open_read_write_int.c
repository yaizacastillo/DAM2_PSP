#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main (int argc, char ** argv){
	
	int fd, fd2 = 1, m, n;
	char buff[4];
	int err = 0;

	if(argc<2){
		printf("Error de paràmetres\n");
		return 1;
	}
	
	//Arxiu de lectura
	fd = open (argv[1], O_RDONLY); //devuelve el fd en el que lo abre (el mas pequeño disponible). abre la ruta con la flag dada
	
	if (fd<0){ //error en obrir arxiu
		perror("Obrint arxiu de lectura\n");
		return 2;	
	}
	
	if(argc>2){ //Arxiu d'escriptura. Si no hi ha, escriurà per pantalla perquè fd2 = 1
		fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC); //obrir per escriure. Si no existeix, el crees. Si existeix, esborres el seu contingut
		
		if(fd2<0){
			perror("Error obrint arxiu d'escriptura\n");
			close (fd);
			return 2;
		}
	}
	
	m = read(fd, buff, 4); //intenta leer del fd, empezando por buff, los bytes indicados. devuelve los bytes leidos, 0 si se ha llegado al final del fichero y -1 si hay un error
	
	while (m>0){ //si no hem arribat al final del fitxer
		write(fd2, buff, m); //escribim el llegit
		m = read(fd, buff, 4); //llegim més
	}
	
	if(m<0) //error de lectura
	{
		perror("Lectura\n");
		err = 3;
	}
	
	close (fd);
	
	return err;

}
