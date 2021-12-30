#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){
	int fd, n, i;
	int findCount = 0;						// target 글자수
	int txtCount = 0;						// text파일 단어의 글자수
	int matched = 0;						// 일치하는 단어수
	char buf[100];
	char tmp;
	int flag;							// 단어가 일치하는지 확인하기 위한 변수


	clock_t start = clock();

	while(*argv[2]){						// 인자로 받은 찾을 단어의 글자수 세기
		findCount++;
		argv[2]++;
	}
	for(int i = 0; i < findCount; i++)				// 인덱스를 글자수만큼 이동하였으므로 다시 원위치
		argv[2]--;


	fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		perror("Open");
		exit(1);
	}

	printf("The target word is '%s' (size = %d)\n", argv[2], findCount);

	while(1){
		while((n = read(fd, &tmp, 1)) > 0){					// fd에서 1글자씩 읽어 tmp에 저장
			txtCount++;							// text파일 단어 글자수가 1증가했다는 의미로 txtOcunt 1증가
			if (tmp == ' ' || tmp == '\n' || tmp == ',' || tmp == '.')	// 단어가 끝나게 되는 문자를 만나면 반복문 탈출
				break;
		}

		if (n == 0)								// n이 0이면 text파일 끝에 도착 -> 반복문 탈출
			break;

		if (n == -1) 								// n이 -1이면 오류발생
			perror("Read");


		if ((txtCount-1) == findCount){						// 찾는 단어와 text파일속 단어의 수가 같으면 
			lseek(fd, (-1)*txtCount, SEEK_CUR);				// 앞에서 read하면서 offset이 이동했으므로 다시 앞으로 이동함
			read(fd, buf, (txtCount - 1));					// fd에서 글자수만큼 다시 읽어 buf에 저장
			printf("\nCompare %s : %s", argv[2], buf);
			
			flag = 1;							// flag를 1로 초기화하고
			for (i = 0; i < findCount; i++){				// 글자수만틈 반복하여
				if(buf[i] != argv[2][i]){				// 다른 글자가 나오면
					flag = 0;					// flag를 0으로 바꾸고 반복문 탈출
					break;	
				}
			}
			if (flag == 1){							// flag가 1이면 단어가 같은 것이므로
				printf(" - Matched!");					// amtched 1증가하고 출력
				matched++;
			}

			lseek(fd, 1, SEEK_CUR);						// " ", "\n", ",", "."을 넘어가도록 offset이동
		}
		txtCount = 0;								// txtCount는 다시 0으로 초기화
	}

	close(fd);

	clock_t end = clock();

	printf("\n\n# of '%s' in the text file is %d", argv[2], matched);
	printf("\nIt takes %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);


	return 0;
}

