#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#define sem_w 0
#define sem_f 1
#define sem_e 2

using namespace std;

int semId, memId;

struct tdata{
	int u, i;
	char m[5];
}*data;

int semSet(int semNum, int semVal){
	return semctl(semId, semNum, SETVAL, semVal);
}

int semOp(int semNum, int semOp){
	sembuf buffer;
	buffer.sem_num = semNum;
	buffer.sem_op = semOp;
	buffer.sem_flg = 0;
	return semop(semId, &buffer, 1);
}

void init(){
	semId = semget(IPC_PRIVATE, 5, 0600);
	memId = shmget(IPC_PRIVATE, sizeof(tdata), 0600);
	cout << "MEMORIJA:::::::::" << semId << endl;
	if(semId==-1 || memId==-1){
		cout << "Greska prilikom alokacije memorije" << endl;
		exit(0);
	}
	data = (tdata*)shmat(memId,NULL,0);
	data->u = 0;
	data->i = 0;
	semSet(sem_w, 1);
	semSet(sem_f, 1);
	semSet(sem_e, 0);
}

void dealoc(int sig){
	semctl(semId, 0, IPC_RMID, 0);
	shmdt((char*)data);
	shmctl(memId, IPC_RMID, NULL);
}

void manufacturer(int proId){
	char temp[20];
	semOp(sem_w,-1);
	cout << "Upisi niz znakova za proizvodjaca " << proId << ": ";
	cin.getline(temp, 20);
	semOp(sem_w,1);
	for(int i=0; true; i++){
		semOp(sem_f, -1);
		semOp(sem_w, -1);
		data->m[(data->u++)%5] = temp[i];
		cout << "Proizvodjac " << proId << "\t-> " << temp[i] << endl;
		sleep(1);
		semOp(sem_w, 1);
		semOp(sem_e, 1);
		if(temp[i]=='\0') break;
	}
}

void customer(){
	char temp[20];
	int end = 0;
	for(int i=0; end!=2; i++){
		semOp(sem_e,-1);
		temp[i] = data->m[(data->i++)%5];
		cout << "Potrosac \t<- " << temp[i] << endl;
		semOp(sem_f, 1);
		if(temp[i]=='\0') end++;
	}
	cout << "\nPrimjeno je: " << temp << endl;
}

int main(){
	sigset(SIGINT, dealoc);
	init();
	for(int i=1; i<3; i++) if(!fork()){ manufacturer(i);  exit(0);}
	if(!fork()){ customer(); exit(0); }
	for(int i=0; i<3; i++) wait(NULL);
	dealoc(1);
	return 0;
}
