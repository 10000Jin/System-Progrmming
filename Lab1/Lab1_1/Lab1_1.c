#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){
	int rfd, wfd, n;
	char buf[2000];
	double tmp;

	int chunkSize = 0;								// chunkSize 0으로 초기화
	while(*argv[3]){
		chunkSize = chunkSize * 10 + (int)(*argv[3]-48);			// 인자로 받은 문자열 chunkSize를 int형으로 변환
		argv[3]++;
	}

	clock_t  start = clock();


	rfd = open(argv[1], O_RDONLY);							// 파일을 엶
	if(rfd == -1){									// 예외처리
		perror(argv[1]);
		exit(1);
	}


	wfd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);			// 새파일 쓰기전용으로 엶 rw-/r--/r--
	if(wfd == -1){
		perror(argv[2]);							// 예외처리
		exit(1);
	}

	while((n = read(rfd, buf, chunkSize)) > 0)					// rfd를 chuckSize만큼 읽어서 buf에 저장, 파일 끝까지 반복
		if(write(wfd, buf, n) != n)						// 기록한 byte수가 읽은 byte수와 다르면 오류문구 출력
			perror("Write");

	if(n == -1)
		perror("Read");

	clock_t end = clock();
	tmp = (double)(end - start) / CLOCKS_PER_SEC;

	printf("Copy %s to %s (Chunk Size = %d)\n", argv[1], argv[2], chunkSize);
	printf("It takes %lf seconds\n", tmp);


	close(rfd);
	close(wfd);

	return 0;
}
