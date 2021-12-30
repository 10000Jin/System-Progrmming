#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


int count = 1;

void sigint_handler(int signo) {
	if (count == 0) {
		printf("아우 먼저\n");
		count++;
	}
	else {
		printf("형님 먼저\n");
		count--;
	}
}

void sigtstp_handler(int signo) {
	//signal(SIGTSTP, SIG_DFL);
	raise(SIGUSR1);
}

void end_handler(int signo) {
	if (count == 1) 
		printf("알았어 형님\n");
	else
		printf("알았어 아우\n");

	exit(1);
}

int main(void) {
	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
	signal(SIGUSR1, end_handler);
	raise(SIGINT);
	while(1) {}

	return 0;
}
