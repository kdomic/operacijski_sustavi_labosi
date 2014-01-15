#include<iostream>
#include<ctime>
#include<signal.h>
#define cekanje 1
#define N 8

using namespace std;

int oznaka_cekanja[N];
int prioritet[N];
int tekuci_prioritet;

void obrada_prekida(int x){
	cout << "Poceo obradu prekida: " << x << endl<<endl;
	for(int i=1; i<6; i++){
		cout << "Obrada prekida " << x  << ": " << i << "/5" << endl;
		sleep(cekanje);
	}
	cout << "Zavrsio obradu prekida: " << x << endl<<endl;
}

void prekidna_rutina(int sig){
	cout << endl;
	time_t *t = new time_t;
	time(t);
	cout << "Prekidna rutina pozvana u: " << ctime(t) << endl;

	int i;
	while(true){
		cout << "Uneiste uzrok prekida: ";
		cin >> i;
		if(i>=1 && i<N) break;
		cout << "Broj mora biti u rasponu 1-" << N << endl;
	}
	oznaka_cekanja[i] = 1;

	int x;
	do{
		x = 0;
		for(int j=tekuci_prioritet+1; j<N; j++)
			if(oznaka_cekanja[j]!=0)
				x = j;

		if(x>0){
			oznaka_cekanja[x] = 0;
			prioritet[x] = tekuci_prioritet;
			tekuci_prioritet = x;
			sigrelse(SIGINT);
			obrada_prekida(x);
			sighold(SIGINT);
			tekuci_prioritet = prioritet[x];
		}
	}while(x>0);

}

void init(){
	tekuci_prioritet = 0;
	for(int i=0; i<N; i++){
		oznaka_cekanja[i] = 0;
		prioritet[i] = 0;
	}
}

int main(){

	init();
	sigset(SIGINT, prekidna_rutina);

	cout << "Poceo osnovni program" << endl;
	for(int i=1; i<31; i++){
		cout << "Glavni program " << i << "/30" << endl;
		sleep(cekanje);
	}
	cout << "Zavrsio osnovni program" << endl;

	cout << endl;
	return 0;
}
