#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void){
    key_t key;
    int shmid;
    int (*shmaddr)[2];                                  // 공유할 배열 포인터

    int powerA = 6;                                     // A와 B의 초기 힘
    int powerB = 3;
    
    srand(time(NULL));                                  // 시간을 seed로 하여 rand 초기화

    key = ftok("shmfile", 1);                           
    shmid = shmget(key, 1024, IPC_CREAT | 0666);        // shared memory 객체 생성
    shmaddr = shmat(shmid, NULL, 0);                    // shared memory를 shmaddr로 매핑
    *shmaddr[0] = 10;                                   // 시작 위치
    *shmaddr[1] = 1;                                    // 몇번째 턴 

    sleep(1);
    while(*shmaddr[0] > 0 && *shmaddr[0] < 20) {        // 누군가 0또는 20에 도달할 때까지
        *shmaddr[0] -= powerA;                          // A팀의 힘만큼 줄 이동
        powerA -= (rand() % 2);                         // A팀의 힘 줄여줌

        *shmaddr[0] += powerB;                          // B팀의 힘만큼 줄 이동
        powerB += (rand() % 2);                         // B팀의 힘 줄어듬
        *shmaddr[1] += 1;                               // 턴 증가
        sleep(2);
    }

    shmdt(shmaddr);                                     // mapping 해제

    return 0;
}