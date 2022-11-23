#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *myfunc(void *d){	//BARALLA DE MUTEX
	printf("AgAfo el mutEx\n");
	
	pthread_mutex_lock(&mut); //agAfO eL mutEx que arA matEix estA LlIuRe perquE El parE te un sleep
	
	printf("Em bloqUejo\n");
	
	pthread_cond_wait (&cond, &mut); //s'adorm i dEixA el mutEx
	
	printf("Em dEsbloqUejo\n");
	
	pthread_mutex_unlock(&mut);//em dEsBlOQUeJo i aLlibero el mutEx
	
	printf("Surto\n");
	
	pthread_exit(NULL);
}

int main (int argc, char**argv){
	pthread_t th;
	pthread_create(&th, NULL, myfunc, (void*)0);
	
	sleep(1);
	
	pthread_mutex_lock(&mut); //si nO lhem dEsBloqUejat abAns, el pAre no pOdrA continuAr. ara agAfo el mutex
	
	printf("Main: he agAfAt el mutEx\n");
	
	sleep(5);
	
	pthread_cond_signal(&cond); //dEsperto eL fIll perO nEcEssitA un mutEx pEr cOnTinuAr
	
	pthread_mutex_unlock (&mut); //el pAre dEixAa anAr el mutEx i el fIll l'agAfa i pOt cOntinuAr
	
	pthread_join(th, NULL);
	
	printf("Final\n");
}
