#include<iostream>
#include<cstdlib>       //atoi, atof, exit
#include<pthread.h>

using namespace std;

int N;
int *flag;
int *num;
int last=0;

void start(int i){
	flag[i] = 1;
	num[i] = ++last;
	flag[i] = 0;
	for(int j=0; j<N; j++){
		while(flag[j]==1);
		while( (num[j]!=0) && (num[j]<num[i] || (num[j]==num[i] && j<i)) );
	}
}

void end(int i){
	num[i] = 0;
}

void * dretva(void *arg){
	int i = *((int*)arg);

	for(int k=1; k<6; k++){
		start(i);
		for(int j=1; j<6; j++){
			cout << "Dretva: " << i+1 << ", "
			     << "K.O.: "   <<  k  << " "
			     << "(" << j << "/5)\n";
			sleep(1);
		}
		end(i);
	}

}

int main(int broj, char *argumenti[]){

        if(broj==1){
            cout << "Nema argumenta!" << endl;
            exit(-1);
        }

        N = atoi(argumenti[1]);
	flag = new int[N];
	num = new int[N];

	pthread_t tid[N];
	for(int i=0; i<N; i++){
		if( pthread_create(&tid[i], NULL, dretva, &i) ){
			cout << "ERROR" << endl;
			exit(-1);
		}
	sleep(1);
	}
	for(int i=0; i<N; i++)
		pthread_join(tid[i], NULL);

        return 0;
}
