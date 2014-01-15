#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

char stanje[6] = {'0','0','0','0','0','\0'};
int vilica[5] = {1,1,1,1,1};
pthread_cond_t red[5];
pthread_mutex_t monitor;

void ispis(int n){ printf("%s (%d)\n", stanje, n+1); }

void misli(int n){ sleep (3); }

void jedi(int n){
        pthread_mutex_lock(&monitor);
        stanje[n] = 'o';
        while (!vilica[n] || !vilica[(n+1)%5]) pthread_cond_wait(&red[n],&monitor);
        vilica[n] = vilica[(n+1)%5] = 0;
        stanje[n] = 'X';
        ispis(n);
        pthread_mutex_unlock(&monitor);
        sleep(2);
        pthread_mutex_lock(&monitor);
        stanje[n] = 'O';
        vilica[n] = vilica[(n+1)%5] = 1;
        pthread_cond_signal(&red[(n-1)%5==-1 ? 4 : &red[(n-1)%5]]);
        pthread_cond_signal(&red[(n+1)%5]);
        ispis(n);
        pthread_mutex_unlock(&monitor);
}

void *filozofi(void *id) {
        int n = *((int*)id);
        while(true){
                jedi(n);
                misli(n);
        }
}

int main (){
        pthread_t tid[5];
        int id[5] = {0,1,2,3,4};
        for(int i=0; i<5; i++)
                if(pthread_create (&tid[i], NULL, filozofi, &id[i])) exit(0);
                
        for(int i=0; i<5; i++)
                pthread_join (tid[i], NULL);

        return 0;
}

