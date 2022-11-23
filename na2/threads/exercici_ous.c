#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX	4
#define OUS 	10

#define TH	5

typedef struct{ //estructura de dades que tindran els threads
	int t[MAX]; //fogons
	int in; //a quin fogo he de posar l'ou (taula circular%MAX) index d'entrada
	int out; //index de sortida
	int num; //opcional. numero dous actualment en ED(estructura de dades)
	int posats;//quantitat d'ous colocats
}t_data;

sem_t sem_ent;
sem_t sem_sort;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *prod(void *d){
	t_data *data = (t_data*)d;
	int fi = 0; //variable local meva, ningu me la pot tocar
	int aux;
	
	while(!fi){
		sem_wait(&sem_ent); //esperar a tenir un forat a la vitro
		printf("(%ld) Desbloquejat de semafor\n", pthread_self()%1000);
		
		pthread_mutex_lock(&mut); //ningu pot canviar-me les variables, l'estructura es compartida i no vull que la toquem a l'hora
		printf("(%ld) Desbloquejat de mutex\n", pthread_self()%1000);
		
		//Estic sol
		if(data->posats== OUS){
			fi = 1;
		} else{
			aux = data->t[data->in] = data->posats+1; //t[data->in] posicio de la taula compartida
			printf("(%ld) He posat l'ou: %d\n", pthread_self()%1000, aux);
			data->in = (data->in+1) % MAX;
			data->num = data->num + 1;
			data->posats = data->posats+1;
		}
		
		pthread_mutex_unlock(&mut);
			
		sem_post(&sem_sort);
		
		int value;
		sem_getvalue(&sem_sort, &value);
		
		printf("(%ld) He acabat: %d\n", pthread_self()%1000,value);
	}
	
	printf("(%ld) Final thread\n", pthread_self()%1000);
}

void *cons(void *d){
	t_data *data = (t_data*)d;
	int fi = 0;
	int aux;
	
	while(!fi){
		sem_wait(&sem_sort); //Esperar a tenir un forat a la vitro
		pthread_mutex_lock(&mut);
		//Estic sol
		if(data->posats == OUS && data->num == 0){
			fi = 1;
		}
		else{
			aux = data->t[data->out];
			printf("He tret l'ou: %d\n", aux);
			data->out = (data->out+1) % MAX;
			data->num = data->num - 1;
		}
		pthread_mutex_unlock(&mut);
			
		sem_post(&sem_ent);
	}


}
int main (int argc, char **argv){
	pthread_t th1[TH], th2;

	t_data data;

	data.in = data.out = data.num = 0;
	data.posats = 0;

	sem_init(&sem_ent, 0, MAX);
	sem_init(&sem_sort, 0, 0);

	for(int i = 0; i <TH; i++){ //Creem els productos
		pthread_create(&th1[i], NULL, prod, (void*)&data);
	}
	pthread_create(&th2, NULL, cons, (void*)&data);

	for(int i = 0; i <TH; i++){
		pthread_join(th1[i], NULL);
	}
	pthread_join(th2, NULL);

	printf("Final\n");

	sem_destroy(&sem_ent);
	sem_destroy(&sem_sort);	
	
	return 0;
}
