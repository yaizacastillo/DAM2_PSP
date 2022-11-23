#include <stdio.h>

int main (int argc, char **argv){ //significa que és un punter a cadenes = *argv[]
	int i;
	
	printf("La funció té %d arguments\n", argc); //cuenta los argumentos que se le pasan al programa. La ejecución del propio programa es el primer argumento.
	
	for(i=0;i<argc;i++){
		printf("L'argument %d és %s\n", i, argv[i]); //argument vector: incluye las cadenas formadas por los argumentos
	}
	
	return argc; //al hacer echo $? nos devuelve el valor (exit status) del último commando
	
}
