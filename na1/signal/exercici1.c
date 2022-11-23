#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <signal.h>
#include <stdlib.h>

int senyals = 0;
int final = 0;

void suma_senyal (int signum){ 
	senyals = senyals + 1;
	printf("%d\n", senyals);
}

void acaba_temps (int signum){ 
	final = 1;
}

int main (int argc, char **argv){
	
	signal(SIGINT, suma_senyal);
	
	signal(SIGALRM, acaba_temps);
	
	alarm(60);
	
	while(!final){
		sleep(1);
	}
	
	printf("He rebut %d senyals\n", senyals);
	
	return 0;
}
