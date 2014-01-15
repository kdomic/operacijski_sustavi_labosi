#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/msg.h>
#include <signal.h>

int msqid;

void end(int a) {
    msgctl(msqid, IPC_RMID, NULL);
    exit(0);
}

struct my_msgbuf {
    long mtype;
    char mtext;
};

int main(){
	sigset(SIGINT, end);
	key_t key = atoi(getenv("MSG_KEY"));
        msqid = msgget(key, 0600 | IPC_CREAT);

	std::cout << "[R] Nadjena varijabla okoline " << getenv("MSG_KEY") <<std::endl;

	struct my_msgbuf buf;

	char text[200];
	int i=0;

	bool wait = true;
	int num = 1;

	while(true){
		if(wait){
        		msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf)-sizeof(long), 0, 0);
			num = 1;
		} else {
			msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf)-sizeof(long), 0, IPC_NOWAIT);
			std::cout << "[R] receiver idle " << num << std::endl;
			num++;
			if(num>5) break;
			sleep(1);
		}

		if(buf.mtext=='\0') wait = false;
		else {
			std::cout << "[R] received: "  << buf.mtext << std::endl;
               		text[i++] = buf.mtext;
			wait = true;
		}
    	}

	text[i++] = '\0';
	std::cout << "[RECEIVED] " << text << std::endl;

	msgctl(msqid, IPC_RMID, NULL) ;
}
