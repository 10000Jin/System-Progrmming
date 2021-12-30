#include "myTimer.h"			// 함수 원형이 정의돈 헤더파일 include
#include <time.h>

int main(void) {
	time_t t;

	myTimer_init(3);		// 타이머 초기화 함수 호출 

	myTimer_on(0);			// 0번 타이머 시작
	myTimer_on(1);			// 1번 타이머 시작
	//do something
        for (int i = 0; i < 300000; i++)
		time(&t);
	myTimer_off(1);			// 1번 타이머 종료


	myTimer_on(2);			// 2번 타이머 시작
	//do something
	for (int i = 0; i < 999999; i++)
		time(&t);
	myTimer_off(2);			// 2번 타이머 종료

	printf("--- 중간결과 ---\n");	
	myTimer_print();		// 중간 결과 출력

        myTimer_on(1);			// 1번 타이머 재시작
	//do something
        for (int i = 0; i < 400000; i++)
		time(&t);
	myTimer_off(1);			// 1번 타이머 종료
	myTimer_off(0);			// 0번 타이머 종료

	printf("--- 최종 결과 ---\n");
	myTimer_print();		// 최종 결과 출력
	myTimer_finalize();		// 메모리 할당 해제
}

