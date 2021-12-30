#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	if (execlp("gcc", "gcc", "-o", argv[2], argv[1], (char *)NULL) == -1) {
					// execlp 호출해 새로운 프로그램 생성(컴파일 프로그램)
		perror("execlp");		// 예외처리
		exit(1);
	}

	return 0;
}
