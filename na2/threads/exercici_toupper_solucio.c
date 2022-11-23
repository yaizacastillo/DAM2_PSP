#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

#define INPUT "input.txt"
#define OUTPUT "output.txt"

typedef struct{
	char c;
	uint8_t usat; //un byte sense signe
}t_info;

sem_t sem1_in;
sem_t sem1_out;
sem_t sem2_in;
sem_t sem2_out;

t_info info1, info2;
int fi = 0;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *llegeix(void *d){

	char c;
	int fd, m;

	fd = open(INPUT, O_RDONLY);
	if(fd<0)
		return NULL;
	
	m = read(fd, &c, 1);
	
	while (m > 0){ //si no hem arribat al final del fitxer
		sem_wait(&sem1_in); //no faré la segona fins que em treguin c
		pthread_mutex_lock(&mut); //unic thread que pot tocar i ja se que c l'han consumit
		
		info1.c = c;
		
		pthread_mutex_unlock(&mut);
		sem_post(&sem1_out);
		m = read(fd, &c, 1);
	}
	
	fi++;
	close(fd);
	return NULL;
}

void *majuscules(void *d){
	int fi_local = 0;
	char c;
	
	while (!fi_local){
		sem_wait(&sem1_out); //ja tinc la dada
		pthread_mutex_lock(&mut);
		
		c = info1.c; //afago la dada
		
		pthread_mutex_unlock(&mut); 
		
		sem_post(&sem1_in); //th1 pot seguir treballant
		
		sem_wait(&sem2_in);
		pthread_mutex_lock(&mut);
		
		info2.c = toupper(c);
		
		if(fi==1)
			fi_local = fi++;
			
		pthread_mutex_unlock(&mut);
		sem_post(&sem2_out);	
	}
}

void *escriu(void *d){
	int fd, m, fi_local = 0;	
	char c;
	
	fd = open(OUTPUT, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(fd<0)
		return NULL;
	
	while (!fi_local){
		sem_wait(&sem2_out);
		pthread_mutex_lock(&mut);
		
		c = info2.c;
		
		pthread_mutex_unlock(&mut);
		sem_post(&sem2_in);
		
		m = write(fd, &c, 1); //m per control d'errors
		
		if(fi==2)
			fi_local = 1;
	}
	
	close(fd);
	return NULL;
}

int main (int argc, char **argv){ //Exercici agafa lletres fitxer, passa a majus i escriu en un altre fitxer
	pthread_t th[3];
	
	sem_init(&sem1_in, 0, 1);
	sem_init(&sem1_out, 0, 0);

	sem_init(&sem2_in, 0, 1);
	sem_init(&sem2_out, 0, 0);
	
	for(int i = 0; i < 3; i++){
		pthread_create(&th[i], NULL, llegeix, NULL);
	}
	
	for(int i = 0; i < 3; i++){
		pthread_join(th[i], NULL);
	}
	
	sem_destroy(&sem1_in);
	sem_destroy(&sem1_out);
	sem_destroy(&sem2_in);
	sem_destroy(&sem2_out);
	
	printf("Final\n");
	
	return 0;
}
