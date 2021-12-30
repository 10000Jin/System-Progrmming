#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int status;					// exit status 저장할 변수
	pid_t pid, pid2;				// 자식 프로세스의 pid를 저장할 변수

	switch (pid = fork()) {				// 자식 프로세스 생성
	    case -1 :					// 예외 처리
			perror("fork");
			exit(1);
			break;
	    case 0 :					// 자식 프로세스라면
			sleep(1);				// 1초간 멈춤
			printf("First Child Process\n");
			printf("My PID : %d\nMy Parent's PID : %d\n\n", (int)getpid(), (int)getppid());
			exit(2);				// pid와 ppid 출력, exit system call호출
			break;
	    default :					// 부모 프로세스라면
	    	switch (pid2 = fork()) {			// 다시 자식 프로세스 생성
   	            case -1 :				
    	       	    perror("fork2");
	    	     	exit(1);
		     		break;
				case 0 :				// (두번째) 자식 프로세스라면
					printf("Second Child Process\n");	
					printf("My PID : %d\nMy Parent's PID : %d\n\n", (int)getpid(), (int)getppid());
					exit(3);				// pid와 ppid 출력, exit system call호출
					break;
				default :				// 부모 프로세스라면
					while (wait(&status) > 0)		// 자식 프로세스들이 종료될 때까지 대기
						continue;			// 실행 중인 자식 프로세스 없음 -> -1 리턴 -> 반복문 탈출
					printf("Parent Process\n");
					printf("My Child's PID : %d, %d\nMy PID : %d\nMy Parent's PID : %d\n", (int)pid, (int)pid2, (int)getpid(), (int)getppid());
					break;				// 자식 프로세스 pid와 부모 pid, ppid 출력
		}
	}

	return 0;
}
