#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>


// 함수 원형
int str2num(char *str);
char *num2str(int num, char *str);
int findPrime(int n, int start, int end);
void print_runtime(int i, struct timeval *t1, struct timeval *t2);


int main(int argc, char* argv[]) {
	FILE *wfp;					// 결과 출력파일
	pid_t ppid;					// 부모 프로세스의 pid
	struct timeval t1, t2;				// 시간 측정용 변수
	int i, n, status;
	int num1, num2, range, startNum, endNum;


	n = str2num(argv[3]);                          	// 프로세스 개수
	num1 = str2num(argv[1]);			// 입력받은 소수 찾는 범위(시작)
	num2 = str2num(argv[2]);                        // 입력받은 소수 찾는 범위(끝)
	range = num2 - num1;				// 소수를 찾을 실제 범위
	i = n;						// 프로세스의 개수 n을 i에 저장(n은 변할 것)
	ppid = (int)getpid();				// 현재(부모)프로세스의 pid저장

	gettimeofday(&t1, NULL);			// 시작 시간
	if ((wfp = fopen(argv[4], "w")) == NULL) {	// 입력받은 파일 fopen
		perror("fopen");
		exit(1);
	}


	while(--n && fork());				// 프로세스 개수-1 만큼 자식 프로세스 생성
							// 부모 프로세스라면 0초과 반환 -> 반복 -> 자식 생성
							// 자식 프로세스라면 0반환 -> while 아래 코드로 
	if (n == 0)					// 마지막 프로세스에 대해
		endNum = num2;				// 끝 범위 -> 마지막 숫자
	else
		endNum = range / i * (i - n);		// 아니라면 범위 나누는 규칙에 따름
	
	if (n == i-1)					// 처음 프로세스라면
		startNum = num1;			// 시작 범위 -> 처음 숫자
	else {
		startNum = range / i * (i - ++n) + 1;	
		n--;					// 윗줄에서 임의로 증가한 n 감소
	}

	printf("[P%d] found %d primes\n", n, findPrime(n, startNum, endNum));
							// findPrime호출해 소수 개수와 서브파일에 소수 출력
	if ((int)getpid() == ppid){			// 부모 프로세스라면
		char buf[BUFSIZ];
		FILE *subFile;
		char filename[BUFSIZ];

		for (int x = i - 1; x >= 0; x--) {	// 3번 파일부터 0번 까지
			subFile = fopen(num2str(x, filename), "r");	// fopen하여 subFile에 파일 포인터 저장
			while (fgets(buf, BUFSIZ, subFile) != NULL) {
				fputs(buf, wfp);	//subFile에 저장된 소수 읽어 결과 파일에 출력
			}
			fclose(subFile);		
			remove(num2str(x, filename));	// 파일 닫고 삭제
		}

		fclose(wfp);				// 결과 파일 닫기
		gettimeofday(&t2, NULL);		// 종료시간
		print_runtime(i, &t1, &t2);		// 실행시간 출력
	}

	wait(&status);					// 부모 프로세스는 자식이 모두 종료될 때까지 대기

	return 0;
}

// 입력받은 범위 정수형으로 변환하는 함수
int str2num(char *str) {
	int num = 0;

	while (*str){                                   // 각 자리의 수에 대해
		num = num * 10 + (int)(*str-48);        // 정수형으로 변환 후 자리수를 곱해
		str++;                                  // 더하는 과정 반복
	}
	return num;
}

// 정수를 문자형 파일명으로 변환하는 함수
char *num2str(int num, char *str){
	int tmp = num;					// 정수 tmp에 저장
	int count = 0;					// 자리수를 나타내는 count

	do {
		tmp /= 10;				// 10으로 나눈 몫 tmp에 저장
		count++;				// 자리수 1증가
	}while(tmp != 0);				// tmp 0이면 반복 종료

	str[count] = '\0';				// 문자열 끝
	do{
		count--;				
		str[count] = (char)(num % 10 + 48);	// 문자형으로 변환한 값 알맞은 위치에 저장
		num = num/10;
	}while(num != 0);				// 0이면 반복 종료
		
	return str;
}

// 소수의 개수를 구하고 서브파일에 출력하는 함수
int findPrime(int n, int start, int end) {
	int i, j, count = 0;                            // i(피제수), j(제수), count(소수 개수)
	FILE *subFile;
	char filename[BUFSIZ];
	subFile = fopen(num2str(n, filename), "w");	// 프로세스 번호로 파일명 엶

	for (i = start; i <= end; i++){                 // 범위 만큼 반복
		for (j = 2; j < i; j++) {               // 2부터 나눔 (1은 모든 수에 대해 나눠 떨어짐)
			if((i % j) == 0)                // 나눠 떨어지면 반복문 탈출
				break;
		}
		if(i == j){                             // 두 수가 같다면 소수
			count++;                        // 같지 않다면 1과 자신 이외의 약수 존재
			fprintf(subFile, "%d ", i);	// subFile에 해당 수(소수)를 씀
		}
	}

	fclose(subFile);				// subFile을 닫음
	return count;					
}

// 실행시간 ms로 변환해 출력하는 함수
void print_runtime(int i, struct timeval *t1, struct timeval *t2) {
	double runtime = 0;                             // 실행시간 변수
	runtime = (t2->tv_sec - t1->tv_sec) * 1000 + (t2->tv_usec - t1->tv_usec) / 1000.0;
							// 실행시간을 ms로 변환
	if (i == 1)
		printf("[Serial] takes %.2lf ms\n", runtime);
	else
		printf("[Parallel] takes %.2lf ms\n", runtime);
}
