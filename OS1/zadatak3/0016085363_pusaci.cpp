#include <iostream>
#include <cstdlib>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

bool duhan, papir, sibice;
sem_t stol, p[3];

void init(){
	sem_init(&stol,0,0);
	for(int i=0; i<3; i++)
		sem_init(&p[i],0,0);
}

void * diller(void *arg){
	while(true){
		duhan = papir = sibice = false;
        	srand(time(NULL)); rand();
	        duhan = rand()%2;
	        if(duhan)
	                if(rand()%2) papir = true;
	                else sibice = true;
	        else{
	                papir = true;
	                sibice = true;
	        }
		cout << "Trgovac stavlja: ";
		if(sibice) cout << "sibice i ";
		if(duhan) sibice ? cout << "duhan" : cout << "duhan i ";
		if(papir) cout << "papir";
		cout << endl;
		for(int i=0; i<3; i++) sem_post(&p[i]);
		sem_wait(&stol);
	}
	pthread_exit(NULL);
}

void * pusac(void *arg){
	int i = *((int*)arg);
	while(true){
		sem_wait(&p[i]);
		switch(i){
			case 0://ima duhan
				if(papir&&sibice){
					cout << "Pusac 1: uzima sastojke i ..." << endl << endl;
					sleep(1);
					sem_post(&stol);
				}
			break;
			case 1://ima papir
				if(duhan&&sibice){
					cout << "Pusac 2: uzima sastojke i ..." << endl << endl;
					sleep(1);
					sem_post(&stol);
				}
			break;
			case 2://ima sibice
				if(duhan&&papir){
					cout << "Pusac 3: uzime sastojke i ..." << endl << endl;
					sleep(1);
					sem_post(&stol);
				}
			break;
		}
	}
	pthread_exit(NULL);
}

void end(int sig){
	sem_destroy(&stol);
	for(int i=0; i<3; i++)
		sem_destroy(&p[i]);
	exit(1);
}

int main(){
	sigset(SIGINT, end);
	init();

	cout << "Pusac 1: ima duhan" << endl;
	cout << "Pusac 2: ima papir" << endl;
	cout << "Pusac 3: ima sibice" << endl << endl;

	pthread_t tid[4];
	pthread_create(&tid[3], NULL, diller, NULL);
	int id[3] = {0,1,2};
	for(int i=0; i<3; i++)
		pthread_create(&tid[i], NULL, pusac, &id[i]);
	for(int i=0; i<4; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
