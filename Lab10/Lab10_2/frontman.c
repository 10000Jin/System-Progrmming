#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

// 문자열 숫자를 정수형으로 변환
int str2num(char *str) {
	int num = 0;

	while (*str){                    		                // 각 자리의 수에 대해
		num = num * 10 + (int)(*str-48);			        // 정수형으로 변환 후 자리수를 곱해
		str++;                                  		    // 더하는 과정 반복
	}
	return num;
}

// 메시지를 담을 구조체
struct quizmsg{
    long mtype;
    char mtext[80];
};

int main(void) {
    key_t key;
    pid_t mypid;
    int msgid;
    struct quizmsg quiz;
    struct msqid_ds m_stat;
    char *problems[] = {"오징어 다리 개수는?", "오징어 게임 최중 우승자 번호는?", "네모 달고나의 꼭짓점 개수는?"};
    int answer[] = {10, 456, 4};

    mypid = getpid();                                                       // 자신의 pid저장

    key = ftok("keyfile", 1);                                               // IPC를 위한 Key생성
    msgid = msgget(key, IPC_CREAT|0644);                                    // 메시지 큐 생성 (Key에 대한 메시지 큐가 없으면 생성)
    if (msgid == -1) {
        perror("msgget"); exit(1);
    }

    printf("ㅇㅅㅁ 오징어 게임 시작 ㅇㅅㅁ\n\n");

    for (int i = 0; i < 3; i++){                                            // 문제수 만큼 반복
        strcpy(quiz.mtext, problems[i]);                                    // 첫번째 문제 메시지 구조체에 복사

        printf(">> %d 라운드 문제 전송\n", i+1);
        quiz.mtype = 1;                                                     // 문제 메시지 타입 설정 (type:1)
        if(msgsnd(msgid, (void *)&quiz, 80, IPC_NOWAIT) == -1){             // 메시지 전송, 메시지 큐가 가득 차도 기다리지 않음
            perror("msgsnd"); exit(1);
        }

        while(1){                                                           // 참가자의 답을 전송 받음
            msgrcv(msgid, &quiz, 80, 2, 0);                                 // 정답 메시지를 전송받음 (type:2))
           
            if(msgctl(msgid, IPC_STAT, &m_stat) == -1) {                    // msgid에 해당하는 msqid_ds를 읽어옴
                perror("msgctl"); exit(1);
            }

            if (str2num(quiz.mtext) == answer[i]){                          // 메시지의 내용이 답과 같다면
                printf("성공!\n%d번 참가자 %d라운드 통과\n\n", m_stat.msg_lspid, i+1);
                break;                                                      // 다음 문제 전송
            }
            else {                                                          // 메시지의 내용이 답과 다르다면
                printf("%d번 참가자 실패\n\n탕!\n", m_stat.msg_lspid);
                msgctl(msgid, IPC_RMID, &m_stat);                           // 메시지 큐 삭제
                kill(m_stat.msg_lspid, SIGKILL);                            // 자신과 참가자 프로세스 종료
                kill(mypid, SIGKILL);
            }
        }
    }
    msgctl(msgid, IPC_RMID, &m_stat);                                       // 문제를 다 맞추고 프로세스 종료시 메시지 큐 삭제
    return 0;
}