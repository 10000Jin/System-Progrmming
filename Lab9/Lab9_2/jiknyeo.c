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

	while (*str){                    		                 // 각 자리의 수에 대해
		num = num * 10 + (int)(*str-48);			 // 정수형으로 변환 후 자리수를 곱해
		str++;                                  		 // 더하는 과정 반복
	}
	return num;
}

int main(int argc, char* argv[]) {
	int pd1, n, a;
	int times;							// 반복 횟수 저장 변수
	char kyunu[20];							// 메시지 저장 배열
	
	if (access("pipeFile", F_OK) == -1 && errno == ENOENT) {	// pipeFile이 존재하지 않으면
		if (mkfifo("pipeFile", 0644) == -1){			// pipeFile 생성
			perror("mkfifo: pipeFile");
			exit(1);
		}
	}

	if ((pd1 = open("./pipeFile", O_WRONLY)) == -1) {		// 쓰기 모드로 pipeFile 오픈
		perror("open pipeFile"); exit(1);
	}

	a = write(pd1, argv[2], strlen(argv[2])+1);			// pipeFile을 통해 반복횟수 전달
	if (a == -1) { perror("write"); exit(1); }


	printf("직녀 =====\n");
	times = str2num(argv[2]);					// 입력받은 반복횟수 정수형 변환


	for (int i = 0; i < times; i++) {				// 반복 횟수만큼 반복
		n = write(pd1, argv[1], strlen(argv[1])+1);		// pipeFile을 통해 메시지 전달
		if (n == -1) { perror("write"); exit(1); }
	
		printf("견우에게: %s\n", argv[1]);	
		sleep(2);
	}

	close(pd1);							// pipeFile 닫아줌

	return 0;
}
