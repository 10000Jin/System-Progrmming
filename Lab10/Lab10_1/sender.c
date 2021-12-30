#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 문자열 숫자를 정수형으로 변환하는 함수
int str2num(char *str) {
	int num = 0;

	while (*str){                    		                // 각 자리의 수에 대해
		num = num * 10 + (int)(*str-48);			        // 정수형으로 변환 후 자리수를 곱해
		str++;                                  		    // 더하는 과정 반복
	}
	return num;
}

// 메시지를 담을 구조체
struct msgbuf {
    long mtype;
    char mtext[80];
};

int main(void) {
    key_t key;
    int msgid;
    char mode[3];
    char input[80];
    struct msgbuf mesg;

    key = ftok("keyfile", 1);                                   // IPC를 위한 Key생성
    msgid = msgget(key, IPC_CREAT|0644);                        // 메시지 큐 생성 (Key에 대한 메시지 큐가 없으면 생성)
    if (msgid == -1) {
        perror("msgget"); exit(1);
    }

    while(1) {
        printf("1. 메일 입력\n2. 종료\n>>> ");
        fgets(mode, sizeof(mode), stdin);                       // 표준입력으로부터 번호 입력받음
        mode[strlen(mode)-1] = '\0';                            // 강제로 널 문자를 넣어줌 (입력버퍼 문제)

        if (str2num(mode) == 2)                                 // 종료
            break;
        else if (str2num(mode) == 1){                           // 메일 작성
            mesg.mtype = 1;                                     // 메시지 타입 설정 (>0)
            printf("\n입력: ");
            fgets(input, sizeof(input), stdin);                 // 메일 내용 입력받음
            printf("\n");
            strcpy(mesg.mtext, input);                          // 입력받은 메일내용 메시지 구조체에 복사

            if(msgsnd(msgid, (void *)&mesg, 80, IPC_NOWAIT) == -1){ // 메일 전송
                perror("msgsnd"); exit(1);
            }
        }
    }

    return 0;
}