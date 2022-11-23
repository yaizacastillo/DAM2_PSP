#include <string.h>
#include <malloc.h>

int main(int argc, char **argv){

	char *mem;
	
	if(argc==1)
		return 1;
		
	mem = (char *)malloc ((strlen(argv[1])+1) * sizeof(char)); //demano memoria malloc per la longitud de la cadena + 0
	
	if(mem==NULL){
		perror("malloc");
		return 2;
	}
	
	strcpy(mem, argv[1]);
	printf("%s\n",mem); //nomes escriu l'argument amb memoria malloc
		
	free(mem);//si s'acaba el programa es llibera la memoria, si no l'hem de liberar
	
	return 0;
}
