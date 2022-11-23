#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int a;
	int b;
} t_data;

void* myfunc (void* d){
	t_data *data = (t_data *)d;
	int *res; //he de guardar la memoria * perque si no quan marxes de la funcio la perderia
	
	res = (int*)malloc(sizeof(int));//estructura de memoria per posar un int
	
	printf("Hola mundo. a=%d b=%d\n", data->a, data->b);
	
	*res = data->a + data->b;
	
	pthread_exit(res);//return del thread que se sincronitza amb el pthread_join
}

int main (int argc, char **argv){
	pthread_t th; //variable thread
	int res;
	int *data_result;
	t_data data;
	
	data.a = 3;
	data.b = 5;
	
	res = pthread_create(&th, NULL, myfunc, (void*)&data);
	data_result = &res;
	
	//pthread_detach(th);//convertir en detach. llavors el join no funciona i el resultat es la merda que hi hagi al punter o el valor que tingui assignat si te un.
	pthread_join(th, (void**)&data_result);//si el pare mor, els creats moren i no fan la seva funcio. esperem el resultat de la funcio. el resultat es un pointer pointer perque vull que em guardi el punter del que vull llegir perque no se de quin tipus sera.
	
	printf("Resultat: %d\n", *data_result);
	
	free(data_result);
	
	return 0;
}

