#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char **argv){ //significa que és un punter a cadenes = *argv[]
	int err = 0;
	
	if(argc < 2) //si no le hemos pasado ningún argumento
	{
		return 1; //se sale devolviendo 1
	}
	
	if(write (1, argv[1], strlen(argv[1])) != strlen(argv[1])){ //escribe en el file descriptor 1 (pantalla), empezando en argv[1], escribe x bytes. devuelve los bytes escritos si ha ido bien, si no -1.
		err = 2;
	}
	
	return err;
	
}
