#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

#define FOGONS	7 //TODO

typedef struct{ //estructura de dades que tindran els threads
	//int fogons; //TODO numero de fogons passats per l'usuari
	int f[FOGONS]; //fogons 
	int t[FOGONS]; //segons per cada fogo
	int ous; //ous totals
	int posats;
	int trets;
	int in; //index on posar ou
	int out; //index on treure ou
	int num; //numero d'ous als fogons
	int max_random; //maxim segons random passats per l'usuari
}t_data;

sem_t sem_prod;
sem_t sem_cons;
sem_t sem_ran;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void* prod(void* d){
	t_data *data = (t_data*)d;
	int final = 0;
	
	while(!final){
		sem_wait(&sem_prod);
		pthread_mutex_lock(&mut);
		printf("(%ld) Sóc un productor\n", pthread_self()%1000);
		if(data->posats==data->ous){
			final++;
		}else{
			data->f[data->in] += data->posats + 1;
			printf("(%ld) He posat l'ou: %d\n", pthread_self()%1000, data->posats + 1);
			data->t[data->in] = -1;
			data->posats += 1;
			data->num += 1;
			data->in = (data->in + 1) % FOGONS; //TODO data->fogons
		}
		
		pthread_mutex_unlock(&mut);
			
		sem_post(&sem_ran);
		
		printf("(%ld) He acabat\n", pthread_self()%1000);
	}
	
	pthread_exit(NULL);
}

void* cons(void* d){
	t_data *data = (t_data*)d;
	printf("(%ld) Sóc un consumidor\n", pthread_self()%1000);
	
	pthread_exit(NULL);
}

void* ran(void* d){
	t_data *data = (t_data*)d;
	srand ( time(NULL) );
	int final = 0;
	int ultima_pos = 0;
	
	while(!final){
		sem_wait(&sem_ran);
		pthread_mutex_lock(&mut);
		printf("(%ld) Sóc un randomitzador\n", pthread_self()%1000);
		if(data->posats == data->ous && data->num == 0){
			final++;
		}
		else{
			for(int i = 0; i < FOGONS; i++){ //TODO data->fogons
				ultima_pos = (ultima_pos + i) % FOGONS; //TODO data->fogons
				if(data->t[ultima_pos] == -1){
					data->t[ultima_pos] = rand()%data->max_random+1;
					break;
				}
			}
		}
		
		for(int i = 0; i < FOGONS; i++){
			printf("%d: %d\n", i, data->t[i]);
		}
		
		pthread_mutex_unlock(&mut);
		sem_post(&sem_cons);
	}
	
	pthread_exit(NULL);
	
}

void* temp(void* d){
	t_data *data = (t_data*)d;
	printf("(%ld) Sóc temporitzador\n", pthread_self()%1000);
	
	/*int final = 0;
	while(!final)	
	{
		pthread_cond_wait (&cond, &mut); //s'adorm i dEixA el mutEx
	
	}
	printf("AgAfo el mutEx\n");
	
	pthread_mutex_lock(&mut); //agAfO eL mutEx que arA matEix estA LlIuRe perquE El parE te un sleep
	
	printf("Em bloqUejo\n");
	
	pthread_cond_wait (&cond, &mut); //s'adorm i dEixA el mutEx
	
	printf("Em dEsbloqUejo\n");
	
	pthread_mutex_unlock(&mut);//em dEsBlOQUeJo i aLlibero el mutEx
	
	printf("Surto\n");
	
	pthread_exit(NULL);*/
	
	pthread_exit(NULL);
}

int main(int argc, char **argv){ // args: productors, consumidors, ous, fogons, max segons
	// si hi ha menys arguments dels necessaris
	if(argc < 6){
		perror("arguments");
		return 1;
	}

	// productors
	int num_productors = atoi(argv[1]);
   	
   	if(num_productors<1){
   		perror("num productors");
   		return 1;
   	}

	// consumidors
	int num_consumidors = atoi(argv[2]);
   	
	if(num_consumidors > num_productors || num_consumidors < 1){
		perror("num consumidors");
		return 1;
	}
	
	// ous
	int total_ous = atoi(argv[3]);
   	
   	if(total_ous<0){
   		perror("num ous");
		return 1;
   	}
   	
   	// fogons
   	int total_fogons = atoi(argv[4]);
   	
   	if(total_fogons<1){
   		perror("num fogons");
		return 1;
   	}
   	
   	// max segons
   	int max_segons = atoi(argv[5]);
   	
   	if(max_segons<1){
   		perror("max segons");
		return 1;
   	}
   	
	printf("Num productors: %d\n", num_productors);
	printf("Num consumidors: %d\n", num_consumidors);
	printf("Num ous: %d\n", total_ous);
	printf("Num fogons: %d\n", total_fogons);
	printf("Max segons: %d\n", max_segons);
	
	pthread_t th_productors[num_productors], th_consumidors[num_consumidors], th_randomitzador, th_temporitzador;
	t_data data;
	
	//data.fogons = total_fogons; //TODO data->fogons
	data.ous = total_ous;
	data.posats = data.trets = data.in = data.out = 0;
	data.max_random = max_segons;
	memset(&data.t, -1, sizeof(data.t));
	
	sem_init(&sem_prod, 0, total_fogons);
	sem_init(&sem_cons, 0, 0);
	sem_init(&sem_ran, 0, 0);
	
	
	for(int i = 0; i <num_productors; i++){ //Creem els productos
		pthread_create(&th_productors[i], NULL, prod, (void*)&data);
	}
	
	for(int i = 0; i <num_consumidors; i++){ //Creem els consumidors
		pthread_create(&th_consumidors[i], NULL, cons, (void*)&data);
	}
	
	//Creem el randomitzador
	pthread_create(&th_randomitzador, NULL, ran, (void*)&data);
	
	//Creem el temporitzador
	pthread_create(&th_temporitzador, NULL, temp, (void*)&data);
	
	for(int i = 0; i <num_productors; i++){
		pthread_join(th_productors[i], NULL);
	}
	
	for(int i = 0; i <num_consumidors; i++){
		pthread_join(th_consumidors[i], NULL);
	}
	
	pthread_join(th_randomitzador, NULL);
	
	pthread_join(th_temporitzador, NULL);
	
	printf("Final\n");
	
	return 0;
}
