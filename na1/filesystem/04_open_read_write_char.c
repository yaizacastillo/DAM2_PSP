#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main (int argc, char ** argv){
	
	int fd, m, n;
	char c;
	int err = 0;

	if(argc<2){
		printf("Error de paràmetres\n");
		return 1;
	}
	
	fd = open (argv[1], O_RDONLY);
	
	if (fd<0){
		perror("Obrint arxiu de lectura\n");
		return 2;	
	}
	
	m = read(fd, &c, 1);
	
	while (m>0){ //si no hem arribat al final del fitxer
		write(1, &c, m); //escribim el llegit
		m = read(fd, &c, 1); //llegim més
	}
	
	if(m<0) //error de lectura
	{
		perror("Lectura\n");
		err = 3;
	}
	
	close (fd);
	
	return err;

}
