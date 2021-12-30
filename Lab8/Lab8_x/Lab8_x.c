#include <sys/mman.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

extern int errno;


// 매핑이 성공하였는지 확인하는 메크로
#define CHECK_MMAP_SUCCESS(_addr)       \
if (_addr == MAP_FAILED)                \
{                                       \
        perror("mmap");                 \
        exit(1);                        \
}


// ctrl-c가 눌렸을때 호출되는 핸들러
void sigint_handler(int signo) {
	if (!(access("tmp.txt", F_OK) == -1 && errno == ENOENT))	// tmp.txt파일이 존재하면 삭제
		remove("tmp.txt");

	exit(0);							// 종료
}


int main(int argc, char* argv[]) {
	int fd, i = 1, flag, count = 0;
	char message[40];						// 발신 메시지를 저장할 배열
	caddr_t addr;

	fd = open("tmp.txt", O_CREAT | O_RDWR | O_APPEND, 0644);	// tmp.txt파일 읽기 쓰기 모드
	if (fd == -1){							// 파일 없을시 생성, 이어서 쓰기로 엶
		perror("tmp.txt");					
		exit(1);
	}
	ftruncate(fd, 200);						// tmp.txt의 파일 크기를 200byte로 늘림

	signal(SIGINT, sigint_handler);					// ctrl-c에 대한 핸들러 등록

	addr = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
					// tmp.txt 파일을 메모리에 매핑, 읽기 쓰기 접근 가능, 매핑 내용 공유
	CHECK_MMAP_SUCCESS(addr);				
	close(fd);							// tmp.txt 파일 닫기


	// 매핑한 메모리의 0번지는 유저에 대한 차례 정보 저장
	if (addr[0] == 0) {						// ftruncate로 0초기화 했으므로
		addr[0] = '0';						// 첫번째 사용자의 경우 0번지값 0
		flag = 0;						// 따라서 첫번째 사용자에 대해
	}								// 0번지를 '0', flag를 0으로 설정

	else 								// 두번째 사용자의 경우
		flag = 1;						// 이미 첫번째 사용자에서 0번지가 
									// '0'로 변경 -> flag 1로 설정
	
	while(1) {							//채팅을 이어 나가기 위한 반복
		while(1){						// Spin lock의 P()연산
			if (addr[0] == flag + 48)			// 0번지와 flag가 같으면
				break;					// 해당 사용자의 차례
		}							// 자기 차례가 오기전까지 무한대기


		// 상대방이 발신한 내용 출력
		if(!(flag == 0 && count == 0)){				// 첫번째 사용자의 첫번째 수신내용을 없음
			printf("[");
			for (; addr[i] != '\0'; i++)			// 1번지부터 출력(i = 1로 초기화함)
				printf("%c", addr[i]);			// 발신자 출력
			printf("] ");
	
			i++;	
			for (; addr[i] != '\0'; i++)			// 메시지 출력
				printf("%c", addr[i]);
		}
	
		// 메시지 발신
		printf(": ");
		fgets(message, sizeof(message), stdin);		// 표준입력으로 입력받은 문자열 message에 저장
	
		for (int j = 0; argv[1][j] != '\0'; j++)		// 발신자 메모리에 작성
			addr[i++] = argv[1][j];				
		addr[i++] = '\0';					// '\0' 추가
	
		for (int j = 0; message[j] != '\0'; j++)		// 메시지 내용 메모리에 작성
			addr[i++] = message[j];

		addr[i] = '\0';						// '\0' 추가
		count++;						// 횟수 1증가
		
		addr[0] = (flag + 1) % 2 + 48;				// Spin lock의 V()연산 
	}								// 0번지를 다음차례로 변경
									// '0'또는 '1'
	return 0;
}
