#include <sys/wait.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define MAX_LENGTH 20
#define CHILD 0

using namespace std;

int main(void) {
	fstream fInput;
	fInput.open("zadaci.txt", ios::in);
	int answer, pipe_input[2], pipe_output[2];
	char ret[MAX_LENGTH] = "";
	char *send = new char[20];

	pipe(pipe_input);
	pipe(pipe_output);

	int type = fork();

	if(type==CHILD){
			close(pipe_input[1]);
			close(pipe_output[0]);
			dup2(pipe_input[0], STDIN_FILENO);
			dup2(pipe_output[1], STDOUT_FILENO);
			execlp("bc", "bc", NULL);			
	} else {
		while(1) {
			fInput.getline(send, 20, '\n');
			if(fInput.eof()) break;			
			send[strlen(send)] = '\n';
			cout << send;
			cin >> answer;
			close(pipe_input[0]);
			close(pipe_output[1]);
			write(pipe_input[1], send, strlen(send));
			read(pipe_output[0], ret, MAX_LENGTH);
			if(!atoi(ret)) cout << "Invalid sendession." << endl;
			else if(atoi(ret) != answer) cout << "Incorrect. Result is: " << ret;
			else  cout << "Correct." << endl;
		}
		fInput.close();
	}
	return 0;
}