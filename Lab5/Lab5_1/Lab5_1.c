#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	pid_t pid, pgid;   			// pid와 pgid를 저장할 변수 선언
	int PGID = 0;				// 입력받은 top프로세스의 pgid를 저장할 변수

	pid = getpid();				// 현재 프로세스의 pid저장
	pgid = getpgid(pid);			// 현재 프로세스의 pgid저장

	printf("PID : %d\n", (int)pid);		// pid와 pgid 출력
	printf("PGID : %d\n", (int)pgid);

	printf("---------\n");
	
	while(*argv[1]){			// 입력받은 top의 문자열 형식의 pid
		PGID = PGID * 10 + (int)(*argv[1]-48);	// 정수형으로 변환
		argv[1]++;
	}

	setpgid(0, PGID);			// 현재 프로세스의 pgid를 top의 것으로 변경
	pgid = getpgid(pid);			// 현재 프로세스의 pgid 다시 저장
		
	printf("PID : %d\n", (int)pid);		
	printf("PGID : %d\n",(int)pgid);

	while(1){}				// 프로세스가 종료되지 않도록 무한루프

	return 0;
}

	
