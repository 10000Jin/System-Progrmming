#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int time = 0;							// 현재 시간 변수
int maxTime = 0;						// 설정한 최대 시간

// 입력받은 초 정수형으로 변환하는 함수 
int str2num(char *str) {
	int num = 0;
		while (*str){                                   // 각 자리의 수에 대해
 			num = num * 10 + (int)(*str-48);        // 정수형으로 변환 후 자리수를 곱해
			str++;                                  // 더하는 과정 반복
		}
		return num;
}

// ctrl-c와 ctrl-z에 대한 시그널 처리
void sig_handler (int _sig) {
	printf("아직 움직일 수 없다!\n");
}

// 1초마다 알람에 의해 실행되는 timer함수
void timer(int _sig) {
	time++;							// 1초 증가
	printf("\n%d초 경과\n", time);				

	if (time < maxTime) {					// 아직 현재시간이 설정한 시간보다 작으면
		alarm(1);					// 1초뒤 다시 timer함수 호출
	}
	else {							// 5초가 되면
		signal(SIGINT, SIG_DFL);			// ctrl-c와 ctrl-z에 대한
		signal(SIGTSTP, SIG_DFL);			// 시그널을 다시 default로 처리
		printf("시간 정지 종료\n\n");
		printf("내가 시간을 %d초나 멈췄다!\n", time);
		printf("이젠 CTRL-C (CTRL-Z)가 활성화 됨!\n");
	}
}


int main(int argc, char* argv[]) {
	printf("시간 정지\n");

	maxTime = str2num(argv[1]);				// 입력받은 문자열 초 정수형 변환
	signal(SIGALRM, timer);					// SIGALRM에 대한 timer함수 등록
	signal(SIGINT, sig_handler);        	        	// ctrl-c와 ctrl-z에 대한
	signal(SIGTSTP, sig_handler);           	        // 시그널을 sig_handler로 처리

	alarm(1);
	
	while(1) pause();					// 시그널을 받기 위해 무한루프

	return 0;
}
