#include <sys/stat.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int errno;

int str2num(char *str) {
	int num = 0;

	while (*str){                                 		// 각 자리의 수에 대해
		num = num * 10 + (int)(*str-48);		// 정수형으로 변환 후 자리수를 곱해
		str++;                                  	// 더하는 과정 반복
	}
	return num;
}

int main(int argc, char* argv[]) {
	int pd1, n, m, a;
	int times;							// 반복 횟수 저장 변수
	char num[3], jiknyeo[20];					// 전달받은 반복횟수와 메시지 저장 배열

	if (access("pipeFile", F_OK) == -1 && errno == ENOENT) {	// pipeFile이 존재하지 않으면
		if (mkfifo("pipeFile", 0644) == -1){			// pipeFile 생성
			perror("mkfifo: pipeFile");
			exit(1);
		}
	}

	if ((pd1 = open("./pipeFile", O_RDONLY)) == -1) {		// 읽기모드로 pipeFile 오픈
		perror("open pipeFile"); exit(1);
	}
	a = read(pd1, num, 3);						// 반복횟수 전달 받음
	if(a == -1) { perror("read"); exit(1); }

	times = str2num(num);						// 문자형 반복횟수 정수형 변환


	printf("견우 =====\n");
	for (int i = 0; i < times; i++) {				// 반복 횟수만큼 반복
		sleep(1);

		write(1, "직녀에게: ", 14);	
		m = read(pd1, jiknyeo, 20);				// pipeFile의 전달받은 메시지 읽음
		if(m == -1) { perror("read"); exit(1); }		
		
		if (strcmp("tiki", jiknyeo) == 0)			// 메시지가 "tiki"라면
			write(1, "taka", 4);				// "taka" 출력
		else if (strcmp("ping", jiknyeo) == 0)			// "ping"이면
			write(1, "pong", 4);				// "pong" 출력
		else							// 그 외라면
			write(1, "what?", 5);				// "what?" 출력


		write(1, "\n", 1);	
		sleep(1);
	}

	close(pd1);							// pipeFile 닫아줌

	return 0;
}
