#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

//문자열 -> 정수
int _atoi(char *str) {
	int result = 0;

	while(*str) {
		result += 10;
		result += *str++ - '0';
	}

	return result;
}

// 정수 -> 문자열
char *_itoa(int num, char *str) {
	int i = 0;
	int radix = 10;		// 진수
	int deg = 1;		// 차수 12345 -> 10000
	int count = 0;		// 자릿수

	// 0 -> '0'은 따로 처리
	if (num == 0) {
		str[0] = '0';
		str[1] = 0;
		return str;
	}

	// 자릿수 측정
	while(1) {
		if ((num / deg) > 0) {
			count++;
		} else {
			// 원래보다 10이 더 곱해졌기 때문에 줄여줌
			deg /= radix;
			break;
		}
		deg *= radix;
	}
	// 큰자리수부터 변환
	for (i = 0; i < count; i++) {
		str[i] = num / deg + '0';	// 한자리씩 문자변환
		num -= (num / deg) * deg;	// int에서 변환된 숫자 제거
		deg /= radix;			// 차수 낮추기
	}

	str[i] = '\0';		// null 추가
	return str;
}


// 1 : true, 0 : false
int isPrime(int num){
	if(num == 1) return 0;
	for (int i = 2; i * i <= num; i++) {
		if (num % i == 0) return 0;
	}
	return 1;
}

// 프로세스들별 하위연산 및 연산결과를 각자 중간파일에 출력
void subOperation(int procid, int subStart, int subEnd) {
	int count = 0;

	// 각 프로세스들은 병렬적으로 소수들을 구함.
	// 소수를 구하면 각자 파일을 생성하여 출력함.
	FILE *subResult;
	char filename[BUFSIZ];
	subResult = fopen(_itoa(procid, filename), "w");

	for(int i = subStart; i <= subEnd; i++) {
		if (isPrime(i)) {
			count++;
			fprintf(subResult, "%d", i);
		}
	}

	printf("[P%d] found %d primes\n", procid, count);

	fclose(subResult);
}

int main(int argc, char **argv) {
	FILE *wfp;
	int start = _atoi(argv[1]);
	int end = _atoi(argv[2]);
	int numOfProc = _atoi(argv[3]);
	pid_t pid = getpid();
	clock_t t_start, t_end;

	//최종결과 파일
	if ((wfp = fopen(argv[4], "w")) == NULL) {
		perror("fopen");
		exit(1);
	}

	// 자식 할당량의 시작, 끝을 이력
	int subStart = start;
	int subEnd = subStart;

	// 각 프로세스가 맡을 크기
	// 전체 사이즈 / 프로세스 개수
	int workSize = ((end - start + 1) / numOfProc);

	// fork를 이용해서 입력받은 프로세스 개수마늠 반복적으로 자식 생성
	// 한 부모가 여러 자식을 가지고 있는 형태
	t_start = clock(); 	// 시간 측정
	for (int i = 0; i < numOfProc; i++) {
		// 끝 수 지정
		subEnd = subStart + workSize;

		// 마지막 프로세스면 나머지 숫자들 모두 처리
		if (i == numOfProc - 1) {
			subEnd = end;
		}

		// 0번 연산묶음은 부모가 처리
		if(i == 0)
			subOperation(i, subStart, subEnd);

		// 프로세스가 더 필요하면 자식 추가
		if(i > 0) pid = fork();
		// 생성된 자식들은 연산 수행후 종료
		if (pid == 0) {
			subOperation(i, subStart, subEnd);
			exit(0);
		}

		// 다음 프로세서 subStart 업데이트
		subStart = subEnd + 1;
	}

	// 부모는 연산이 끝나도 wait() 통해 모든 자식이 끝나는 것을 기다리고
	while (wait(NULL) != -1) {
		continue;
	}

	// 자식들의 파일을 하나로 합침.
	char buf[BUFSIZ];
	FILE *subResult;
	char filename[BUFSIZ];

	// 중간파일 하나씩 열어서 최종파일에 복사
	for (int i = 0; i < numOfProc; i++) {
		subResult = fopen(_itoa(i, filename), "r");
		while (fgets(buf, BUFSIZ, subResult) != NULL) {
			fputs(buf, wfp);
		}
		fclose(subResult);
	}
	t_end = clock();

	// 시간측정 메시지 출력
	if (numOfProc == 1)
		printf("[Serial] ");
	else if (numOfProc > 1)
		printf("[Parallel] ");

	printf("takes %.2lf ms\n", ((double)(t_end - t_start) / CLOCKS_PER_SEC) * 1000);

	// 부모가 자원해제
	if (pid > 0) {
		fclose(wfp);
		// 중간저장용 파일 삭제
		for (int i = 0; i < numOfProc; i++) {
			remove(_itoa(i, filename));
		}
	}

	return 0;
}



































