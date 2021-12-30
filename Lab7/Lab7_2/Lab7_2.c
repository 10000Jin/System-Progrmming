#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int a = 0;							// 형과 아우의 차례 변수 a

// ctrl-c에 대한 핸들러
void sigint_handler(int signo) {
	int num;						// 차례를 저장할 변수
	num = a % 2;						// a를 2로 나눈 나머지 (0 또는 1) num저장

	if (num == 0)						// 0이면 (형님 차례)
		printf("아우 먼저\n");
	else 							// 1이면 (아우 차례)
		printf("형님 먼저\n");

	a++;							// a 1증가
}

// ctrl-z에 대한 핸들러
void sigtstp_handler(int signo) {
	int num2;						// 현재 차례에 대한 변수
	num2 = a % 2;						// a를 2로 나눈 나머지 (0 또는 1) num2저장

	if (num2 == 1)						// 1이면 (현재 아우)
		printf("알았어 형님\n");
	else							// 0이면 (현재 형님)
		printf("알았어 아우\n");

	exit(0);
}

int main(void) {
	printf("형님 먼저\n");

	if (signal(SIGINT, sigint_handler) == SIG_ERR) {	// ctrl-c에 대한 핸들러 등록
		fprintf(stderr, "Cannot handle SIGINT!\n");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR) {	// ctrl-z에 대한 핸들러 등록
		fprintf(stderr, "Cannot handle SIGTSTP!\n");
		exit(EXIT_FAILURE);
	}

	while(1) pause();					// 시그널을 받기 위해 무한루프

	return 0;
}
