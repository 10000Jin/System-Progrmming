#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){
	int rfd, wfd, n;
	char buf;

	clock_t start = clock();

	rfd = open(argv[1], O_RDONLY);					// 파일 읽기 전용으로 엶 
	if(rfd == -1){
		perror(argv[1]);
		exit(1);
	}

	wfd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);	// 새파일 쓰기전용으로 얾 rw-/r--/r--
	if(wfd == -1){
		perror(argv[2]);
		exit(1);
	}


	while(1){						
		n = read(rfd, &buf, 1);					// buf에 1글자씩 읽어서 저장
		
		if (n == -1){						// n이 -1이면 오류
			perror("Read");
			exit(1);
		}

		if (n == 0)						// n이 0이면 파일 끝에 도착 -> 반복문 탈출
			break;
		
		if (buf > 31 && buf < 128){				// 글자의 아스키 값이 32~127이면 
			printf("%c", buf);				// 문자 출력
			if (write(wfd, &buf, n) != n){			// 그리고 wfd에 씀
				perror("Wirte");
				exit(1);
			}
		}

		else 							// 아스키 값에 해당하지 않는 다면
			lseek(rfd, buf%32, SEEK_CUR);			// buf%32만큼 offset이동하여 다시 반복
	
	}

	close(rfd);
	close(wfd);

	clock_t end = clock();
	printf("\nIt takes %lf seconds\n", (double)(end-start) / CLOCKS_PER_SEC);

	return 0;
}
