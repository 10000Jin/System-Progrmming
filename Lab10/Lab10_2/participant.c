#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 메시지를 담을 구조체
struct quizmsg{
    long mtype;
    char mtext[80];
};

int main(void) {
    key_t key;
    int msgid;
    char answer[10];
    struct quizmsg quiz;

    key = ftok("keyfile", 1);                                   // IPC를 위한 Key 생성
    msgid = msgget(key, 0);                                     // 메시지 큐 생성
    if (msgid == -1) {
        perror("msgget"); exit(1);
    }

    printf("ㅇㅅㅁ 당신은 오징어 게임에 참가하셨습니다. ㅇㅅㅁ\n\n");

    for (int i = 0; i < 3; i++) {                               // 문제수 만큼 반복
        msgrcv(msgid, &quiz, 80, 1, 0);                         // 문제를 전송받음 (type:1)
        printf(">> %d라운드. %s: ", i+1, quiz.mtext);            // 전송받은 문제 출력
        scanf("%s", answer);                                    // 답 입력받음
        printf("\n");

        strcpy(quiz.mtext, answer);                             // 입력받은 답 메시지 구조체에 복사
        quiz.mtype = 2;                                         // 정답 메시지 타입 설정 (type:1)
        if(msgsnd(msgid, (void *)&quiz, 80, IPC_NOWAIT) == -1){ // 답 전송
            perror("msgsnd"); exit(1);
        }
        sleep(1)                                                // 1초 대기하지 않으면
;    }                                                          // 문제를 전송받기 전에 답입력 문구가 출력됨.
    printf("456억원 확득!!!\n");

    return 0;
}
