#include<iostream>
#include<cstdlib>       //atoi, atof, exit
#include<unistd.h>      //fork
#include<sys/wait.h>    //wait
#include<cmath>
#define pi 4*atan(1)

using namespace std;

struct tpolje{
        int n;
        float a;
}*polje = new tpolje[5];

void opseg(tpolje x){
        cout << "n=" << x.n << "\ta=" << x.a << "\tO=" << x.n*x.a << endl;
}

void povrsina(tpolje x){
        cout << "n=" << x.n << "\ta=" << x.a << "\tP=" << 0.25*x.n*x.a*x.a/tan(pi/x.n) << endl;
}

void program(int x){
        povrsina(polje[x]);
        sleep(1);
        opseg(polje[x]);
        exit(0);
}

int main(int broj, char *argumenti[]){

        const int M = broj-1;
        const int N = M/2;

        if(M%2 || !M || N>5){
                cout << "Broj argumenata mora biti paran i u rasponu [2,10]" << endl;
                return 0;
        }

        int poz=1;
        for(int i=0; i<N; i++){
                polje[i].n = atoi(argumenti[poz++]);
                polje[i].a = atof(argumenti[poz++]);
        }

	for(int i=0; i<N; i++)
                switch(fork()){
                case 0:
                        program(i);
                        break;
                case -1:
                        cout << "Greska" << endl;
                        exit(-1);
                        break;
		default: sleep(1);
        	}

	for(int i=0; i<N; i++)
		wait(NULL);

        return 0;
}

