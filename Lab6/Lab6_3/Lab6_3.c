#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

//함수 원형
int str2num(char *str);
void print_time(int a, int b, int pid);
double runtime(struct timeval *tv1, struct timeval *tv2);
int findPrime(int start, int end);


int main(int argc, char* argv[]) {
	int startNum, endNum, midNum;			// 소수를 검사하려는 범위의 변수
	struct timeval t1, t2, t3, t4, t5, t6;		// 실행시간 측적을 위한 변수
	pid_t pid;					

	startNum = str2num(argv[1]);			// 인자로 받은 문자열 범위 정수형 변환
	endNum = str2num(argv[2]);			// str2num 함수 호출
	
	// Serial algorithm	
	print_time(0, 0, (int)getpid());		// 시간을 출력하는 print_time함수 호출
							// (serial(0) or parallel(1), start(0) or end (1), pid)
	gettimeofday(&t1, NULL);
	printf("[Serial] found %d primes\n", findPrime(startNum, endNum));    // 소수 개수 구하는 findPrime함수 호출
	gettimeofday(&t2, NULL);
	printf("[Serial] takes %.2lf ms\n", runtime(&t1, &t2));        // 실행 시간을 ms로 반환하는 runtime함수 호출
	print_time(0, 1, (int)getpid());	


	// Parallel algorithm
	print_time(1, 0, (int)getpid());		// 시간 출력
	midNum = (startNum + endNum) / 2;		// 두개의 프로세스에게 일을 나누기 위해 범위 반 나눔

	switch (pid = fork()){				// 자식 프로세스 생성
	    case -1 :					// 예외 처리
		perror("fork");
		exit(1);
		break;
	    case 0 :					// 자식 프로세스인 경우
		gettimeofday(&t3, NULL);
		printf("[PID = %d] I found %d prime numbers ", (int)getpid(), findPrime(startNum, midNum-1));
		printf("between (%d ~ %d)\n", startNum, midNum-1);
							// 범위안 소수 개수와 범위 출력
		gettimeofday(&t4, NULL);
		printf("[PID = %d] takes %.2lf ms\n", (int)getpid(), runtime(&t3, &t4));
		print_time(1, 1, (int)getpid());	// 실행시간과 프로세스 종료 시간, pid 출력
		break;
	    default :					// 부모 프로세스인 경우
		gettimeofday(&t5, NULL);
		printf("[PID = %d] I found %d prime numbers ", (int)getpid(), findPrime(midNum, endNum));
		printf("between (%d ~ %d)\n", midNum, endNum);
							// 범위안 소수 개수와 범위 출력
		gettimeofday(&t6, NULL);
		printf("[PID = %d] takes %.2lf ms\n", (int)getpid(), runtime(&t5, &t6));
		print_time(1, 1, (int)getpid());	// 실행시간과 프로세스 종료 시간, pid 출력
		break;
	}

	return 0;
}


// 입력받은 범위 정수형으로 변환하는 함수
int str2num(char *str) {
	int num = 0;

	while (*str){					// 각 자리의 수에 대해
		num = num * 10 + (int)(*str-48);	// 정수형으로 변환 후 자리수를 곱해
		str++;					// 더하는 과정 반복
	}
	return num;
}


// 시작&종료 시간을 출력하는 함수
void print_time(int a, int b, int pid) {		// (serial(0) or parallel(1) , start(0) or end(1) , pid)
	struct tm *tm;					// 시간 저장을 위한 구조체 선언
	time_t t;
	char buf[257];					// 형식에 맞는 날짜를 저장할 buf 선언

	time(&t);
	tm = localtime(&t);				// 한국 기준

	if (a == 0){					// Serial 이면
		if (b == 0)				// start 이면
			printf("[Serial start] ");
		else					// end 이면
			printf("[Serial end] ");
	}
	else {						// Parallel 이면
		if (b == 0)				// start 이면
			printf("[Parallel start] ");
		else 					// end 이면
			printf("[Proc.%d end] ", pid);
	}

	strftime(buf, sizeof(buf), "%a %b %e %T %G", tm);	// strtime으로 tm을 형식에 맞게 buf에 저장
	printf("%s\n", buf);					// 출력
}


// 실행시간 ms로 반환하는 함수
double runtime(struct timeval *t1, struct timeval *t2) {
	double runtime = 0;				// 실행시간 변수

	runtime = (t2->tv_sec - t1->tv_sec) * 1000 + (t2->tv_usec - t1->tv_usec) / 1000.0;
							// 실행시간을 ms로 변환
	return runtime;
}


// 소수의 개수를 구하는 함수
int findPrime(int start, int end) {
	int i, j, count = 0;				// i(피제수), j(제수), count(소수 개수)
	
	for (i = start; i <= end; i++){			// 범위 만큼 반복
		for (j = 2; j < i; j++) {		// 2부터 나눔 (1은 모든 수에 대해 나눠 떨어짐)
			if((i % j) == 0)		// 나눠 떨어지면 반복문 탈출
				break;
		}
		if(i == j)				// 두 수가 같다면 소수
			count++;			// 같지 않다면 1과 자신 이외의 약수 존재
	}
	return count;
}











