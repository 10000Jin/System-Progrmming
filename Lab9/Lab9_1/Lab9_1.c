#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	FILE *fp;						// file pointer 변수

	fp = popen("./sample.out", "w");			// pipe stream 열기 (쓰기 모드)
	if(fp == NULL) {
		fprintf(stderr, "popen failed\n");
		exit(1);
	}

	fprintf(fp, "무~야~호~~~~~~!\n");			// formatted IO 사용해 메시지 전달

	pclose(fp);						// pipe stream 닫아줌

	return 0;
}
