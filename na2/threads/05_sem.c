#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
void *myfunc1(void* d){
	int i;
	int id = pthread_self()%1000;

	for(i=0; i < 5; i++){
		sem_wait(&sem);
		
		printf("Sóc el thread %d i entro\n", id);
	
		sleep(1);
	
		printf("Sóc el thread %d i surto\n", id);
	
		sem_post(&sem);
		
		sleep(1); //sense el sleep, al altre thread potser li donaria temps a agafar el semafor o potser el tornari a agafar el mateix thread
	}
}

int main (int argc, char **argv){
	pthread_t th1, th2;
	
	//Init sem
	sem_init(&sem, 0, 1); //només podrà entrar un thread
	
	pthread_create(&th1, NULL, myfunc1, (void*)0);
	pthread_create(&th2, NULL, myfunc1, (void*)0);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	
	printf("Final\n");
	
	return 0;
}
