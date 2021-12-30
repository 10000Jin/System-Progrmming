#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

// 문자형 숫자를 정수형으로 변환하는 함수
int str2num(char *str) {
    int num = 0;

    while(*str){
        num = num * 10 + (int)(*str-48);
        str++;
    }

    return num;
}

// semctl의 마지막 인자를 처리하기 위한 공용체
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// 세마포어 키를 받아 세마포어를 만들고 초기화하는 함수
int initsem(key_t semkey) {
    union semun semunarg;
    int status = 0, semid;

    semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0600);             // 세마포어를 만듦. (변수 1개)
    if(semid == -1) {
        if (errno == EEXIST)
            semid = semget(semkey, 1, 0);
    }
    else {
        semunarg.val = 1;
        status = semctl(semid, 0, SETVAL, semunarg);                    // 세마포어 값을 1로 초기화
    }

    if (semid == -1 || status == -1) {perror("initsem"); return(-1);}

    return semid;
}

// P()연산 - 잠금기능
int semlock(int semid) {
    struct sembuf buf;

    buf.sem_num = 0;                                    // 세마포어 1개이므로 0번
    buf.sem_op = -1;                                    // 음수를 주어 잠금기능, 1번에 1명만 들어갈 수 있음. 따라서 -1
    buf.sem_flg = SEM_UNDO;                             // 프로세스가 비정상적으로 종료될 시, 동작 취소
    if (semop(semid, &buf, 1) == -1) {                  // 세마포어 실행 (명령의 개수 1개)
        perror("semlock failed"); exit(1);
    }
    return 0;
}

// V()연산 - 잠금해제 기능
int semunlock(int semid) {
    struct sembuf buf;

    buf.sem_num = 0;                                    // 세마포어 1개이므로 0번
    buf.sem_op = 1;                                     // 양수를 주어 잠금해제, 1번에 1명만 들어갈 수 있음. 따라서 1
    buf.sem_flg = SEM_UNDO;                             // 프로세스가 비정상적으로 종료될 시, 동작 취소
    if (semop(semid, &buf, 1) == -1) {                  // 세마포어 실행 (명령의 개수 1개)
        perror("semunlock failed"); exit(1);
    }
    return 0;
}

// 세마포어의 잠금과 해제가 있는 테스트를 위한 함수
void semhandle(FILE** _fp) {
    int semid;
    pid_t pid = getpid();

    if((semid = initsem(1)) < 0) exit(1);                   // 세마포어를 만들고 초기화 

    semlock(semid);                                         // P()연산 - CS진입
    fprintf(*_fp, "%d ", pid);                              // pid를 paper.txt에 적음 (Critical Section)
    sleep(1);
    printf("%d번 손님 대기 등록 완료했습니다.\n", pid);       
    semunlock(semid);                                       // V()연산 - CS탈출
    exit(0);
}

int main(int argc, char* argv[]) {
    int a, times;
    FILE *fp;

    if ((fp = fopen("paper.txt", "w")) == NULL) {           // paper.txt 파일 엶. (쓰기 전용)
        perror("fopen"); exit(1);
    }

    times = str2num(argv[1]);                               // 인자로 받은 반복횟수 정수로 변환
    printf("번호표를 발급합니다.\n");
    for(a = 0; a < times; a++)                              // 반복횟수만큼 자식 프로세스 생성
       if(fork() == 0) semhandle(&fp);                      // 자식 프로세스에 대해 semhandle함수 호출 paper.txt의 fp를 매개변수로 전달
    
    while (wait(NULL) != -1) {                              // wait() 통해 모든 자식이 끝나는 것을 기다림
		continue;
	}

    fclose(fp);                                             // paper.txt를 닫음
    return 0;
}