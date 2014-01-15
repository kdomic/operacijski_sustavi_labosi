#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

struct tdata{
	char name[10];
	int txt;
}*data;
int n, Num, Hash;
bool end;
string buffer;
fstream dat;
sem_t sem0, sem1, sem2;

void init(int argc, char *argv[]){
	end = false;
	Num = 0;
	Hash = 0;
	n = argc-1;
	data = new tdata[n];
	int j=0;
	for(int i=1; i<=n; i++){
		if(!strcmp(strrchr(argv[i],'.'),".txt"))
			data[j].txt = 1;
		else if(!strcmp(strrchr(argv[i],'.'),".html"))
			data[j].txt = 0;
		else{
			cout << "Analiza obustavljana za datoteku \"" << argv[i] << "\"" << endl;
			continue;
		}
		strcpy(data[j++].name,argv[i]);
	}
	n = j;

	sem_init(&sem0,0,0);
	sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);
}

void * fetch(void *arg){
	if(!dat.is_open()) return 0;
  	while(true){
		getline(dat,buffer);
		if(dat.eof()) break;
		sem_post(&sem1);
		sem_post(&sem2);
		sem_wait(&sem0);
		sem_wait(&sem0);
 	}
	end = true;
	sem_post(&sem1);
	sem_post(&sem2);
	pthread_exit(NULL);
}

void * wordCount(void *arg){
	while(true){
		sem_wait(&sem1);
		if(end) break;
		for(int i=0; i<buffer.length(); i++)
			if(isalnum(buffer[i])){
				Num++;
				while(isalnum(buffer[++i]));
			}
		sem_post(&sem0);
	}
	pthread_exit(NULL);
}

void * tagCount(void *arg){
	while(true){
		sem_wait(&sem1);
		if(end) break;
		for(int i=0; i<(buffer.length()-1); i++)
			if(buffer[i]=='<' && isalnum(buffer[i+1]))
				Num++;
		sem_post(&sem0);
	}
	pthread_exit(NULL);
}

void * hash(void *arg){
	while(true){
		sem_wait(&sem2);
		if(end) break;
		for(int i=0; i<buffer.length(); i++)
			Hash ^= buffer[i];
		sem_post(&sem0);
	}
}

void analyze(int i){
	dat.open(data[i].name, ios::in);
	pthread_t tid[3];
	pthread_create(&tid[0], NULL, fetch, NULL);
	if(data[i].txt)
		pthread_create(&tid[1], NULL, wordCount, NULL);
	else
		pthread_create(&tid[1], NULL, tagCount, NULL);
	pthread_create(&tid[2], NULL, hash, NULL);

	for(int j=0; j<3; j++)
		pthread_join(tid[j],NULL);

	cout << "Datoteka \"" << data[i].name << "\" ima " << Num << " ";
	data[i].txt ? cout << "rijeci; " : cout << "tagova; ";
	cout << "zastitna suma je: " << Hash << endl;
}

int main(int argc, char *argv[]){
	if(argc<2) return 0;
	init(argc, argv);

	for(int i=0; i<n; i++)
		switch(fork()){
			case 0:
			   analyze(i);
			   exit(0);
			case -1:
			   exit(0);
		}
	for(int i=0; i<n; i++)
		wait(NULL);
	sem_destroy(&sem0);
	sem_destroy(&sem1);
	sem_destroy(&sem2);
	return 0;
}
