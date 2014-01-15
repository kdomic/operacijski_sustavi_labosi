#include <iostream>
#include <cstdlib>
#include <sys/msg.h>
#include <unistd.h>

struct my_msgbuf {
    long mtype;
    char mtext;
};

int main(int argc, char* argv[]){
	key_t key = atoi(getenv("MSG_KEY"));
        int msqid = msgget(key, 0600 | IPC_CREAT);

	std::cout << "[S] Nadjena varijabla okoline " << getenv("MSG_KEY") <<std::endl;

	struct my_msgbuf buf;

	for(int i=0; true; i++){
		buf.mtext = argv[1][i];
		msgsnd(msqid, (struct msgbuf *)&buf, 1, 0);
		if(argv[1][i]=='\0') break;
		sleep(1);
	}
	std::cout << "[S] Sender ends!" << std::endl;
	return 0;
}
