#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX	100000

typedef struct{
	long mtype;
	long moment;
	long id;
	int ou;
	int fogo;
	int temps;
	char* comanda;
}t_msg;


typedef struct{
	int temps;
	int ou;
}t_fogo;

typedef struct{ //estructura de dades que tindran els threads
	time_t begin; //moment en que comença el programa
	t_fogo f[MAX]; //fogons
	int fogons;
	int ous; //ous totals
	int max_random; //maxim segons random passats per l'usuari
	int in; //a quin fogo he de posar l'ou (taula circular%MAX) index d'entrada
	int out; //index de sortida
	int num; //opcional. numero dous actualment en ED(estructura de dades)
	int posats;//quantitat d'ous colocats
}t_data;

sem_t sem_prod;
sem_t sem_cons;
sem_t sem_ran;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *prod(void *d){
	t_data *data = (t_data*)d;
	int fi = 0; //variable local meva, ningu me la pot tocar
	int aux;
	long now;
	
	while(!fi){
		sem_wait(&sem_prod);
		pthread_mutex_lock(&mut);
		if(data->posats== data->ous){
			fi++;
		} else{
			for(int i = 0; i < data->fogons; i++){
				data->in = (data->in + i) % data->fogons;
				if(data->f[data->in].ou == 0){
					aux = data->f[data->in].ou = data->posats+1;
					data->f[data->in].temps = -1;
					now = time(NULL)-data->begin;
					printf("[%ld](%ld) PROD: He posat l'ou %d al fogo %d\n", now,pthread_self()%1000, aux, data->in+1);
					data->in = (data->in+1) % data->fogons;
					data->num = data->num + 1;
					data->posats = data->posats+1;
					sem_post(&sem_ran);
					break;			
				}
			}
			
		}
		
		pthread_mutex_unlock(&mut);
	}
	sem_post(&sem_prod);
	printf("(%ld) PROD: Final thread\n", pthread_self()%1000);
	pthread_exit(NULL);
}

void *cons(void *d){
	t_data *data = (t_data*)d;
	int fi = 0;
	int aux;
	long now;
	
	while(!fi){
		sem_wait(&sem_cons);
		pthread_mutex_lock(&mut);
		
		if(data->posats == data->ous && data->num == 0){
			fi++;
		}
		else{
			for(int i = 0; i < data->fogons; i++){
				if(data->f[data->out].temps == 0 && data->f[data->out].ou > 0){
					aux = data->f[data->out].ou;
					data->f[data->out].ou = 0;
					now = time(NULL)-data->begin;
					printf("[%ld](%ld) CONS: He tret l'ou %d del fogo %d\n", now, pthread_self()%1000, aux, data->out+1);
					data->out = (data->out+1) % data->fogons;
					data->num = data->num - 1;
					sem_post(&sem_prod);
					break;
				}
				data->out = (data->out + 1) % data->fogons;
			}
			
		}
		pthread_mutex_unlock(&mut);
	}
	sem_post(&sem_cons);
	printf("(%ld) CONS: Final thread\n", pthread_self()%1000);
	pthread_exit(NULL);
}
	
void* ran(void* d){
	t_data *data = (t_data*)d;
	srand ( time(NULL) );
	int final = 0;
	int ultima_pos = 0;
	long ultima_crida = 0;
	long now;
	int fd;
	char b[512];
	int feina;
	
	if((fd = open("p2.log", O_WRONLY | O_CREAT | O_TRUNC))<0){
		perror("obrir log");
		close (fd);
	}
	
	while(!final){
		sem_wait(&sem_ran);
		ultima_crida = now;
		pthread_mutex_lock(&mut);
		now = time(NULL)-data->begin;
		if(now-ultima_crida > 1){
			printf("[%ld](%ld) RAN: Més d'un segon sense cridar-me\n", now, pthread_self()%1000);
			sprintf(b,"[%ld](%ld) RAN: Més d'un segon sense cridar-me\n", now, pthread_self()%1000);
			if(write(fd, b, strlen(b)) != strlen(b)){
				perror("Error d'escriptura en el log");
				close(fd);
			}

		}
		feina = 0;
		for(int i = 0; i < data->fogons; i++){
			
			if(data->f[ultima_pos].temps == -1 && data->f[ultima_pos].ou > 0){
				feina++;
				data->f[ultima_pos].temps = rand()%data->max_random+1;
				printf("[%ld](%ld) RAN: He posat temps a l'ou %d del fogo %d: %d\n", now, pthread_self()%1000, data->f[ultima_pos].ou, ultima_pos+1, data->f[ultima_pos].temps);
				ultima_pos = (ultima_pos + 1) % data->fogons;
				break;
			}
			
			ultima_pos = (ultima_pos + 1) % data->fogons;
		}
		
		if(!feina && data->posats == data->ous){
			final++;
		}
		
		pthread_mutex_unlock(&mut);
	}
	
	close(fd);
	printf("(%ld) RAN: Final thread\n", pthread_self()%1000);
	pthread_exit(NULL);
	
}

void* temp(void* d){
	t_data *data = (t_data*)d;
	int fi = 0;
	long now;
	
	while(!fi){
		printf("-----------------%d\n",data->num);
		if(data->posats == data->ous && data->num == 0){
			fi++;
		}
		else{
			sleep(1);
			pthread_mutex_lock(&mut);
			for(int i = 0; i < data->fogons; i++){
				if(data->f[i].temps > 0){
					data->f[i].temps--;
					now = time(NULL)-data->begin;
					printf("[%ld](%ld) TEMP: Ou %d. Fogo %d. Temps %d\n", now, pthread_self()%1000, data->f[i].ou, i+1, data->f[i].temps);
					
					if(data->f[i].temps == 0){
						sem_post(&sem_cons);
					}
				}
				
				
			}
		}
		
		pthread_mutex_unlock(&mut);
		
	}
	sem_post(&sem_prod);
	sem_post(&sem_cons);
	sem_post(&sem_ran);
	printf("(%ld) TEMP: Final thread\n", pthread_self()%1000);
	pthread_exit(NULL);
}

int main(int argc, char **argv){ // args: productors, consumidors, ous, fogons, max segons //TODO: pas de missatges per ipc
	int pid, msgid;
	t_msg msg;

	// si hi ha menys arguments dels necessaris
	time_t begin = time(NULL);
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
   	
	if(num_consumidors < 1){
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
   	
   	if(total_fogons<1 || total_fogons > MAX){
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
	
	data.begin = begin;
	data.fogons = total_fogons;
	data.productors = num_productors;
	data.consumidors = num_consumidors;
	data.ous = total_ous;
	data.in = data.out = data.num = data.posats = 0;
	data.max_random = max_segons;
	for(int i = 0; i < total_fogons; i++){ //inicialitzem tots els temps de fogons a -1
		data.f[i].temps = -1;
	}
	for(int i = 0; i < total_fogons; i++){ //inicialitzem tots fogons a buits
		data.f[i].ou = 0;
	}
	
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
	
	//Creació msg
	if((msgid = msgget(IPC_PRIVATE, IPC_CREAT))<0){
		perror("msgget");
		return 1;
	}
	
	if((pid = fork())<0){
		perror("fork")
		return 1;
	}
	
	if(pid == 0){
	
	}
	
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
