#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


void *myfunc1 (void *d) {
int res;

   res = pthread_mutex_trylock (&mut); //com mutex lock pero si el mutex ja està agafat, igualment fa la seva feina sense bloquejar-se, pero la funció no retorna 0
   
   printf ("Soc el thread 1. Entro: %d\n", res);

   sleep (1); 
   
   printf ("Soc el thread 1. Surto\n");

   pthread_mutex_unlock (&mut);


   pthread_exit (NULL);
}


void *myfunc2 (void *d) {
int res;

   res = pthread_mutex_trylock (&mut);

   printf ("Soc el thread 2. Entro: %d\n", res);
   
   sleep (1); 
   
   printf ("Soc el thread 2. Surto\n");

   pthread_mutex_unlock (&mut);

   
   pthread_exit (NULL);
}


int main (int argc, char **argv) {
pthread_t th1, th2;

   pthread_create (&th1, NULL, myfunc1, (void *)NULL);
   pthread_create (&th2, NULL, myfunc2, (void *)NULL);

   pthread_join (th1, NULL);
   pthread_join (th2, NULL);

   printf ("Final\n");
   
return 0;
}
