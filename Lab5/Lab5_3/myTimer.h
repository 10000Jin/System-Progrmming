#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

// 전역변수
struct timeval *timer; 					// 시간정보를 저장할 구조체 선언
double result[10] = {0};				// 결과를 저장할 배열 0으로 초기화해 선언
int num = 0;						// 타이머의 개수 변수 선언


int myTimer_init(int numTimers) {			// timer 초기화 함수
	num = numTimers;				// 매개변수로 받은 타이머 개수 num에 저장
	timer = (struct timeval *)malloc(sizeof(struct timeval) * num*2);	
 							// 타이머 개수*2 크기만큼 배열 동적 할당
	return 0;					// 타이머 하나당 시작시간과 종료시간 저장해야하기 때문
}

int myTimer_finalize(void){				// timer 메모리 할당 해제 함수
	free(timer);					// timer배열 메모리 반납

	return 0;
}

int myTimer_on(int timerID) {				// 타이머 시작 함수
	gettimeofday(&timer[2 * timerID], NULL);	// 기준시간부터 경과한 시간 배열에 저장

	return 0;
}

int myTimer_off(int timerID) {				// 타이머 종료 함수
	gettimeofday(&timer[2 * timerID + 1], NULL);	// 시간 배열에 저장

	result[timerID] += (timer[2*timerID+1].tv_sec - timer[2*timerID].tv_sec) * 1000 \
	+ (timer[2*timerID+1].tv_usec - timer[2*timerID].tv_usec) / 1000.0;
							// 타이머 시간을 ms단위로 맞춰 result에 저장
	return 0;	
}

int myTimer_print(void) {				// 타이머 시간을 출력하는 함수
	for (int i = 0; i < num; i++) {			// 모든 타이머 출력
		if (result[i] == 0)			// 0초라면
			printf("Timer %d : 타이머가 시작되지 않았거나 runnig중입니다.\n", i);
		else					
			printf("Timer %d : %.2lf ms\n", i, result[i]);
	}

	return 0;
}
