#include <stdio.h>
#include <time.h>

#define HOUR 3600						// 1시간 = 60초 * 60분

int checkStr(char *input, char *local) {			// 입력받은 문자열과 각 시간대 문자열 비교 함수
	int i = 0;						// 문자열의 인덱스
	int check = 1;						// 문자열이 일치하는지 확인하는 변수

	while (input[i] != '\0' || local[i] != '\0'){		// 두 문자열의 끝까지 비교
		if (input[i] != local[i]) {
			check = 0;				// 다른 부분이 나오면 check를 0으로 바꾸고 탈출
			break;
		}
		i++;						// 같으면 다음 문자 비교
	}

	return check;						// check 반환
}


int main(int argc, char *argv[]) {
	struct tm *tm;						// 시간 정보 저장할 구조체를 가르킬 포인터 변수
	time_t t;						// 시간 정보 저장할 변수
	char buf[256];						// 출력할 시간 정보 문자열을 저장할 배열
	
	time(&t);						// 시간(초)를 얻어옴
	tm = localtime(&t);					// 한국 기준으로 초를 구조체로 변환
	
	printf("KST : ");
	strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);	// 해당 형식으로 buf에 저장
	printf("%s\n", buf);						// 시간을 형식대로 출력


	if (checkStr(argv[1], "GMT")){				// 입력 문자열이 GMT에 해당하면
		tm = gmtime(&t);				// UTC기준 구조체로 변환
		printf("GMT : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);	// buf에 저장후 출력
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "JST")) {			// 입력 문자열이 JST에 해당하면
		t += (HOUR * 9);				// UTC기준 +9시간 시차
		tm = gmtime(&t);
		printf("JST : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "CST")) {			// 입력 문자열이 CST에 해당하면
		t += (HOUR * 8);				// UTC기준 +8시간 시차
		tm = gmtime(&t);
		printf("CST : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "MST")) {			// 입력 문자열이 MST에 해당하면
		t -= (HOUR * 7);				// UTC기준 -7시간 시차
		tm = gmtime(&t);
		printf("MST : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "BRT")) {			// 입력 문자열이 BRT에 해당하면
		t -= (HOUR * 3);				// UTC기준 -3시간 시차
		tm = gmtime(&t);
		printf("BRT : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "CEST")) {			// 입력 문자열이 CEST에 해당하면
		t += (HOUR * 2);				// UTC기준 +2시간 시차
		tm = gmtime(&t);
		printf("CEST : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "EAT")) {			// 입력 문자열이 EAT에 해당하면
		t += (HOUR * 3);				// UTC기준 +3시간 시차
		tm = gmtime(&t);
		printf("EAT : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	else if (checkStr(argv[1], "AEST")) {			// 입력 문자열이 AEST에 해당하면
		t += (HOUR * 10);				// UTC기준 +10시간 시차
		tm = gmtime(&t);
		printf("AEST : ");
		strftime(buf, sizeof(buf), "[%m월 %d일 %a %l:%M %p]", tm);
		printf("%s\n", buf);
	}

	return 0;
}


