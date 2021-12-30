#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	char buf[20];						// 전달받은 메시지가 저장될 배열

	printf("무~한~~~~~~!\n");

	sleep(1);
	scanf("%s", buf);					// 메시지 입력받음 (buf에 저장)
	printf("%s", buf);					// 메시지 출력
	printf("\n");

	return 0;
}
