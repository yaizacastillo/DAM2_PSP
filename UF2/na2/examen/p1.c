#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	int num1;
	int num2;
	char solucio;
	int res_fitxer;
	int res_calculat;
	int final;
}t_data;

sem_t sem_in;
sem_t sem_comp;
sem_t sem_cal;
sem_t sem_guar;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *f1(void *d){
	t_data *data = (t_data*)d;
	FILE* file = fopen ("entrada.txt", "r");
	char *split;
	char line [1000];
	int arr[4];
	int i = 0;
	if (file != NULL) {
		while(fgets(line,sizeof line,file)!= NULL) {
			sem_wait(&sem_in);
			split = strtok(line, " ");
			i = 0;
			while(split){
				arr[i] = atoi(split);
				split = strtok(NULL," ");
				i++;
			}
			
			for(int j = i; j<4; j++){
				arr[j] = -99;
			}
			pthread_mutex_lock(&mut);
			data->num1 = arr[0];
			data->num2 = arr[1];
			if(arr[2]!=-99){
				data->solucio = 'S';
				data->res_fitxer = arr[2];
				sem_post(&sem_comp);
				
				printf("LECTOR: Passo al comparador els numeros %d + %d = %d\n", data->num1, data->num2, data->res_fitxer);
			}
			else{
				data->solucio = 'N';
				sem_post(&sem_cal);
				printf("LECTOR: Passo al calculador els numeros %d + %d\n", data->num1, data->num2);
			}
			pthread_mutex_unlock(&mut);
		}
		pthread_mutex_lock(&mut);
		data->final++;
		pthread_mutex_unlock(&mut);
	}
	else{
		perror("fitxer entrada"); //print the error message on stderr.
	}
	//Quan acabo crido la resta per si ningú els crida perquè acabin
	sem_post(&sem_comp);
	sem_post(&sem_cal);
	sem_post(&sem_guar);
	printf("LECTOR: He acabat\n");
	fclose(file);
	pthread_exit(NULL);
}

void *f2(void *d){
	t_data *data = (t_data*)d;
	int final = 0;
	int num1, num2, res_fitxer, res;
	while(!final){
		
		sem_wait(&sem_comp);
		pthread_mutex_lock(&mut);
		if(data->final>0){
			final++;
		}
		num1 = data->num1;
		num2 = data->num2;
		res_fitxer = data->res_fitxer;
		res = num1 + num2;
		data->res_calculat = res;
		pthread_mutex_unlock(&mut);
		sem_post(&sem_guar);
		if(res == res_fitxer){
			printf("COMP: Resultat correcte\n");
		}
		else{
			printf("COMP: Resultat incorrecte\n");
		}
	}
	
	printf("COMP: He acabat\n");
	pthread_exit(NULL);
}

void *f3(void *d){
	t_data *data = (t_data*)d;
	int final = 0;
	int num1, num2, res_fitxer, res;
	while(!final){
		sem_wait(&sem_cal);
		pthread_mutex_lock(&mut);
		if(data->final>0){
			final++;
		}
		num1 = data->num1;
		num2 = data->num2;
		res = num1 + num2;
		data->res_calculat = res;
		printf("CAL: EL resultat és: %d\n", data->res_calculat);
		pthread_mutex_unlock(&mut);
		sem_post(&sem_guar);
	}
	printf("CAL: He acabat\n");
	pthread_exit(NULL);
}

void *f4(void *d){
	int final = 0;
	t_data *data = (t_data*)d;
	FILE *file= fopen("resultat.txt","w");
	int num[4];
	char res;
	while(!final){
		sem_wait(&sem_guar);
		pthread_mutex_lock(&mut);
		if(data->final>0){
			final++;
		}
		num[0]= data->num1;
		num[1]=data->num2;
		num[2]=data->res_fitxer;
		num[3]=data->res_calculat;
		res = data->solucio;
		pthread_mutex_unlock(&mut);
		sem_post(&sem_in);
		
		fprintf(file,"%d %d ",num[0], num[1]);
		if(res == 'S'){
			fprintf(file,"%d %d\n",num[2], num[3]);
		}
		else{
			fprintf(file,"- %d\n", num[3]);
		}
		
	}
	printf("GUAR: He acabat\n");
	fclose(file);
	pthread_exit(NULL);
}

int main (int argc, char **argv){
	pthread_t th1, th2, th3, th4;
	t_data data;
	data.final = 0;
	
	sem_init(&sem_in, 0, 1);
	sem_init(&sem_comp, 0, 0);
	sem_init(&sem_cal, 0, 0);
	sem_init(&sem_guar, 0, 0);
	
	pthread_create(&th1, NULL, f1, (void*)&data);
	pthread_create(&th2, NULL, f2, (void*)&data);
	pthread_create(&th3, NULL, f3, (void*)&data);
	pthread_create(&th4, NULL, f4, (void*)&data);
	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);

	printf("Final\n");

	sem_destroy(&sem_in);
	sem_destroy(&sem_comp);
	sem_destroy(&sem_cal);
	sem_destroy(&sem_guar);	
	
	return 0;
}
