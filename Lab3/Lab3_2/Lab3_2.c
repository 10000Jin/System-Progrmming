#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>


#define YEAR 31536000					// 60초*60분*24시간*365일
#define MONTH 2592000					// 60초*60분*24시간*30일
#define DAY 86400					// 60초*60분*24시간
#define HOUR 3600					// 60초*60분
#define MINUTE 60					// 60초


int total = 0;
int maxLen = 0;


//total 값 구하기
void sumBlocksize(char *fname) {
	struct stat buf;				// 파일의 정보 저장할 구조체 선언
	stat(fname, &buf);				// 파일의 정보 buf에 저장

	total += (int)buf.st_blocks;			// total에 블록 개수 더함(전역변수)

	int i = 1;
	int len = 0;
	while ((int)buf.st_size > i) {			// checkFilesize()함수에서 출력 포맷을 맞추기 위해
		i *= 10;				// 최대 사이즈 길이 maxLen을 구함 
		len++;					// 자리수가 오를때마다 len 1증가
	}

	if (len > maxLen) 				// 파일의 사이즈가 maxLen보다 크면
		maxLen = len;				// 갱신
       	
}


// 파일 유형, 권한 검사 함수
void checkType(char *fname) {				
	struct stat buf;
	stat(fname, &buf);
	int kind;					// 파일의 종류 저장할 변수

	
	//파일 유형 검사
	kind = buf.st_mode & S_IFMT;			// 파일의 종류 구해 kind에 저장
	switch (kind) {					// switch문으로 해당하는 파일 유형 출력
		case S_IFDIR:
			printf("d"); break;
		case S_IFLNK:
			printf("l"); break;
		case S_IFCHR:
			printf("c"); break;
		case S_IFBLK:
			printf("b"); break;
		case S_IFREG:
			printf("-"); break;
	}

	//파일 권한 검사
	//사용자 권한 검사
	if (buf.st_mode & S_IRUSR)			// 소유자 읽기 권한이 있으면
		printf("r");				// 'r'출력
	else
		printf("-");				// 없으면 '-'출력
	if (buf.st_mode & S_IWUSR)			// 소유자 쓰기 권한이 있으면
		printf("w");
	else
		printf("-");
	if (buf.st_mode & S_IXUSR)			// 소유자 실행 권한이 있으면
		printf("x");
	else
		printf("-");

	//그룹 권한 검사
	if (buf.st_mode & S_IRGRP)
		printf("r");
	else
		printf("-");
	if (buf.st_mode & S_IWGRP)
		printf("w");
	else
		printf("-");
	if (buf.st_mode & S_IXGRP)
		printf("x");
	else
		printf("-");

	//기타 사용자 권한 검사
	if (buf.st_mode & S_IROTH)
		printf("r");
	else
		printf("-");
	if (buf.st_mode & S_IWOTH)
		printf("w");
	else
		printf("-");	
	if (buf.st_mode & S_IXOTH)
		printf("x");
	else
		printf("-");

}


void checkHardlinks(char *fname) {				// 하드 링크 개수를 구하는 함수
	struct stat buf;
	stat(fname, &buf);

	printf(" %o ", (unsigned int)buf.st_nlink);		// st_nlink로 해당 파일의 하드 링크수 출력
}


void checkFilesize(char *fname) {				// 파일 사이즈를 구하는 함수
	struct stat buf;
	stat(fname, &buf);

	int i = 1;
       	int len = 0;
	
	while ((int)buf.st_size > i) {				// 해당 파일의 사이즈 길이를 구함
		i *= 10;
		len++;						// 자리수가 올라갈때마다 len 1증가
	}

	for (int i = 0; i < maxLen - len; i++)			// 최대 파일 사이즈 길이에서 현재 파일
		printf(" ");					// 사이즈 길이 차이 만큼 공백 출력
	printf(" %d ", (int)buf.st_size);			// 파일 사이즈 출력
}



void checkModifytime(char *fname) {				// 파일 마지막 수정시간 구하는 함수
	struct stat buf;
	stat(fname, &buf);
	
	// 달의 영문과 12달의 마지막 날을 각 배열에 저장
	char* Months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	int Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	int time, year, day, hour, minute;
	int start = 1970;
	int leapYear = 0;
	
	time = (int)buf.st_mtime;				// 마지막 수정시간 time에 저장
	time += 32400;						// 유닉스 타임에서 한국은 9시간 더함. 

	day = time / DAY;
	year = day / 365 + start;
	hour = (time % DAY) / HOUR;
	minute = (time % HOUR) / MINUTE;


	while (start < year + 1){				// 1970년 부터 윤년 구하기
		if (((start % 4) == 0 && (start % 100) != 0) || (start % 400) == 0)
					// 4로 나눠 떨어지면서 100으로 나눠 떨어지거나 400으로 나눠 떨어지는 년도
			leapYear += 1;				// 윤년 1증가
		start += 1;					// 다음 년도
	}

	day = (day - leapYear + 1) % 365;			// 윤년에 대한 고려
	int i = 0;
	while (1) {
		if ((day - Days[i]) <= 0) {			// 해당 달의 마지막 날을 뺀 값보다 작으면
			printf(" %s %2d", Months[i], day);	// 해당 달의 영문과 날짜 출력
			break;
		}
		else {
			day -= Days[i];				// 아니라면 해당 달의 마지막 날 빼고
			i += 1;					// 다음 달로 이동
		}
	}
	printf(" %02d:%02d", hour, minute);			// 날짜 출력
}

	

int main(void) {
        DIR *dp; 					// 열 폴더의 포인터를 저장할 포인터 변수 선언
  	struct dirent *dent;				// 디렉토리 파일 정보를 저장할 구조체 선언
	struct stat buf;				// 파일의 정보를 저장할 구조체 선언


	if ((dp = opendir(".")) == NULL){		// 현재 폴더를 엶.
		perror("opendir : Current Directory");	// 오류 발생시 예외처리
		exit(1);
	}


	while ((dent = readdir(dp)) != NULL) {		// total을 구하기 위해 미리 폴더의 모든 파일을 읽음 
		lstat(dent->d_name, &buf);		// 파일의 정보를 buf에 저장

		sumBlocksize(dent->d_name);		// 블록 개수를 모두 더하는 함수 호출
	}
	printf("total %d\n", total / 2);
	rewinddir(dp);					// offset이 마지막으로 가있으므로 원위치


	while ((dent = readdir(dp)) != NULL) {		// 나머지 정보를 구하기 위해 폴더의 모든 파일을 읽음
		lstat(dent->d_name, &buf);

		if (S_ISDIR(buf.st_mode) != 1) {	// 폴더가 아닌 파일만을 대상(. .. 제외)
			checkType(dent->d_name);	// 파일 종류와 권한을 검사하는 함수 호출
			checkHardlinks(dent->d_name);	// 하드 링크 수를 구하는 함수 호출
			checkFilesize(dent->d_name);	// 파일 사이즈를 구하는 함수 호출
			checkModifytime(dent->d_name);	// 수정 시간을 구하는 함수 호출
			printf(" %s", dent->d_name);	// 파일명 출력
			printf("\n");
		}
	}


	closedir(dp);					// 파일 닫아줌

	return 0;
}

