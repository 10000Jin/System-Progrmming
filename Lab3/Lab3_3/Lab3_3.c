#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int changeSize(char *size){					// 문자열 샘플 사이즈를 정수형으로 변환 함수 
	int samplingSize = 0;					// 샘플 사이즈 0초기화

	while(*size){
		samplingSize = samplingSize * 10 + (int)(*size-48);	// 정수형으로 변환
		size++;
	}

	return samplingSize;					// 샘플 사이즈 반환
}


void getPrefix(char *input, char *dataDir, char *prefix){	// ~/FullData/data에서 prefix 분리하는 함수
	int i = 0, a = 0;
	int arr[6] = {0};					// '/'가 나오는 인덱스 저장하는 배열
	int max = 0;
	
	for (int j = 0; input[j] != '\0'; j++){			// ~/FullData/data의 끝까지 검사
		if (input[j] == '/') {				// '/'가 나오면
			arr[i] = j;				// 그때의 인덱스 arr에 저장
			i++;					// 마지막 '/'를 기준으로 분리되기 때문
		}
	}

	for (int k = 0; k < 6; k++) {				// arr 크기 만큼 반복
		if(arr[k] > max)				// '/'가 있는 인덱스가 max보다 작으면
			max = arr[k];				// max 갱신
	}


	for (int m = 0; m < max; m++) 				// max(마지막 '/')보다 앞은
		dataDir[m] = input[m];				// dataDir에 저장(~/FullData)
	dataDir[max] = '\0';

	for (int n = max+1; input[n] != '\0'; n++) {		// max이후 (prefix)
		prefix[a] = input[n];				// prefix에 저장
		a++;
	}
	prefix[a] = '\0';
}


int getLen(char *arr){						// 배열의 길이를 구하는 함수
	int len = 0;
	for (int i = 0; arr[i] != '\0'; i++)			// 문자열의 끝까지 반복
		len++;						// lem 1증가
	return len;
}


char *addDir(char *first, char *second){			// 경로를 합치는 함수(출력 경로)
	int len = getLen(first);				// ~/FullData의 길이 구하는 함수 호출
	int i = 0;

	first[len] = '/';					// ~/FullData 마지막에 /추가
	first[len + 1] = '\0';

	for (i = 0; second[i] != '\0'; i++)			// s100 끝까지 반복
		first[len + 1 + i] = second[i];			// ~/FullData/뒤에 이어 붙임
	first[len + 1 + i] = '\0';

	return first;						// 생성된 경로 반환
}

void getLinkName(char *linkName, char *prefix, int j) {		// 링크 이름을 설정하는 함수
	int k = 0, len = 0, s = 1;
	
	len = getLen(prefix);					// prefix의 길이 구하는 함수 호출
	for (int m = 0; m < len; m++) 				// prefix("data")를 링크 이름에 저장
		linkName[m] = prefix[m];

	int x = j;
	while (x != 0){						// 이름의 번호의 자리수 k에 저장
		x = x / 10;
		++k;
	}

	for (int i = 0; i < 3-k; i++)				// 해당 자리수 이외 앞자리는 0으로 채움
		linkName[i + 4] = '0';

	for (int i = 0; i < k; i++)
		s *= 10;

	for (int i = 0; i < k; i++) {				// 이름의 번호를 자리수에 맞게 넣음
		s /= 10;
		linkName[i + 4 + 3-k] = (j/s) + '0';
		j %= s;
	}
}



int main(int argc, char* argv[]) {
	DIR *in_dp;						// open할 입력 디렉토리의 포인터를 저장할 변수
	DIR *out_dp;						// open할 출력 디렉토리의 포인터를 저장할 변수
	struct dirent *dent;					// 디렉토리 파일 정보를 저장할 구조체 선언

	int i = 0, j = 1;
	int samplingSize = changeSize(argv[2]);			// samplingSize를 구하는 함수 호출

	char dataDir[50];					// 데이터가 들어 있는 경로
	char prefix[8];						// prefix 저장할 배열
	char linkName[10];					// link 이름을 저장할 배열

	clock_t start = clock();				// 시작시간 체크

	getPrefix(argv[1], dataDir, prefix);			// prefix를 구하는 함수 호출
								// ~/FullData와 data로 분리 저장
			
        if ((in_dp = opendir(dataDir)) == NULL) {		// ~/FullData 폴더 엶.
		perror("opendir input Dir");			// 실패시 예외처리
		exit(1);
	}

	addDir(dataDir, argv[3]);				// 경로를 합치는 함수 호출 
								// ~/FullData/s100 경로 생성

        if (mkdir(dataDir, 0755) == -1) {			// s100 폴더를 만듦.
		perror("Fail : mkdir s100");			// 예외처리
		exit(1);
	}	

	if ((out_dp = opendir(dataDir)) == NULL) {		// 출력 폴더 엶.
		perror("Fail : opendir s100");			// 예외처리
		exit(1);
	}

	
	chdir(dataDir);						// working directory 출력 폴더로 이동
	seekdir(in_dp, 2);					// (. ..)무시하기 위해 offset이동

	while ((dent = readdir(in_dp))) {			// 파일을 하나씩 읽으면서
		if (i % samplingSize == 0) {			// 샘플 사이즈로 나눠 떨어지는 = 100번 마다
			getLinkName(linkName, prefix, j);	// soft링크 이름을 구하는 함수
			symlink(dent->d_name, linkName);	// soft링크 생성
			j++;
		}
		i++;
	}

	closedir(in_dp);					// 파일을 닫아줌
	closedir(out_dp);

	clock_t end = clock();					// 종료시간 체크
	printf("It takes %lf seconds.\n", (double)(end-start) / CLOCKS_PER_SEC);	// 실행시간 출력

	return 0;
}
