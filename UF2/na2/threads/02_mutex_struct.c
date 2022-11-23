#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
   pthread_mutex_t mut;
} t_data;


void *myfunc1 (void *d) {
int res;
t_data *data = (t_data *)d;

   res = pthread_mutex_lock (&data->mut); //retorna 0 si va bé, número si hi ha un error
   
   printf ("Soc el thread 1. Entro: %d\n", res);

   sleep (1); 
   
   printf ("Soc el thread 1. Surto\n");

   pthread_mutex_unlock (&data->mut);


   pthread_exit (NULL);
}


void *myfunc2 (void *d) {
int res;
t_data *data = (t_data *)d;

   res = pthread_mutex_lock (&data->mut); //retorna 0 si va bé, número si hi ha un error

   printf ("Soc el thread 2. Entro: %d\n", res);

   sleep (1); 
   
   printf ("Soc el thread 2. Surto\n");

   pthread_mutex_unlock (&data->mut);

   
   pthread_exit (NULL);
}


int main (int argc, char **argv) {
pthread_t th1, th2;
t_data data;


   pthread_mutex_init (&data.mut, NULL);
   
   pthread_create (&th1, NULL, myfunc1, (void *)&data);
   pthread_create (&th2, NULL, myfunc2, (void *)&data);

   pthread_join (th1, NULL);
   pthread_join (th2, NULL);

   printf ("Final\n");
   
return 0;
}
