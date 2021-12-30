#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    key_t key;
    int shmid, turn;
    int (*shmaddr)[2];                                  // 공유하는 배열 포인터

    key = ftok("shmfile", 1);
    shmid = shmget(key, 1024, 0);                       // shared memory 객체 생성
    shmaddr = shmat(shmid, NULL, 0);                    // shared memory를 shmaddr로 매핑

    turn = *shmaddr[1];                                 // shmaddr[1]에서 현재의 턴을 읽어 저장

    while(*shmaddr[0] > 0 && *shmaddr[0] < 20) {        // 누군가 0또는 20에 도달할 때까지
        printf("phase : %2d |A", turn);
        for (int i = 0; i < *shmaddr[0]; i++)           // 줄의 위치 앞뒤로 '=' 출력
            printf("=");
        printf("||");                                   // 줄의 위치에 '||' 출력
        for (int i = *shmaddr[0]+1; i <= 20; i++)
            printf("=");
        printf("B\n");
        turn++;                                         // 턴 증가
        sleep(2);
    }

    if (*shmaddr[0] <= 0)                               // 0에 도달(A팀 승리)
        printf("A team win!\n");
    else if(*shmaddr[0] >= 20)                          // 20에 도달(B팀 승리)
        printf("B team win!\n");

    shmdt(*shmaddr);                                    // mapping 해제
    shmctl(shmid, IPC_RMID, NULL);                      // shmid에 해당하는 shared memory 삭제

    return 0;
}