#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include <pthread.h>
#include <semaphore.h>

#define MAX	4

typedef struct{
	int t[MAX]; //segons per cada fogo
}t_data;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* temp(void* d){
	
	int final = 0;
	int timeInMs = 1000;

	printf("(%ld) Sóc temporitzador\n", pthread_self()%1000);
	
	pthread_mutex_lock(&mut);
	
	printf("Em bloqUejo\n");
	
	while(!final){
		struct timespec timeToWait;
		struct timeval now;
	    	int rt;

	    	gettimeofday(&now,NULL);


	    	timeToWait.tv_sec = now.tv_sec+1;
	    	timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;
	    	
	    	pthread_cond_timedwait(&cond, &mut, &timeToWait);
	    	
	    	printf("hola\n");
	}
	
	pthread_mutex_unlock(&mut);
	
	printf("Surto\n");
	
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	
	pthread_t th_temporitzador;
	t_data data;
	
	data.t[0] = 4;
	data.t[1] = 2;
	data.t[2] = 1;
	data.t[3] = 7;
	
	//Creem el temporitzador
	pthread_create(&th_temporitzador, NULL, temp, (void*)&data);
	
	pthread_join(th_temporitzador, NULL);
	
	printf("Final\n");
	
	return 0;
}
