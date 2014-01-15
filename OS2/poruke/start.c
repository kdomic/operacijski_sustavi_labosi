#include <iostream>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

using namespace std;

int main(int argc, char* argv[]){
	if(argc<2){
		cout << "Nedovoljan broj argumenata" << endl;
		return -1;
	}

	setenv("MSG_KEY", "12345", true);
	key_t key = atoi(getenv("MSG_KEY"));
	int msqid = msgget(key, 0600 | IPC_CREAT);

	for(int i=0; i<(argc-1); i++)
		switch(fork()){
                case 0:
			char path[80];
  			strcpy (path,"./sender ");
  			strcat (path, argv[i+1]);
                        system(path);
                        exit(0);
                case -1: cout << "Error" << endl; exit(-1); break;
        	}

	switch(fork()){
		case 0: system("./receiver"); exit(0);
                case -1: cout << "Error" << endl; exit(-1); break;
	}

	for(int i=0; i<argc; i++) wait(NULL);


}
