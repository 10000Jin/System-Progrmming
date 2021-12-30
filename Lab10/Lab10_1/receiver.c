#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

// 메시지를 담을 구조체
struct msgbuf{
    long mtype;
    char mtext[80];
};

int main(void) {
    struct msgbuf inmsg;
    struct msqid_ds m_stat;
    key_t key;
    int msgid;
    int mode;

    key = ftok("keyfile", 1);                                       // IPC를 위한 Key생성
    msgid = msgget(key, 0);                                         // 메시지 큐 생성
    if (msgid == -1) {
        perror("msgget"); exit(1);
    }
    
    while (1) {
        if(msgctl(msgid, IPC_STAT, &m_stat) == -1) {                // msgid에 해당하는 msqid_ds를 읽어옴
        perror("msgctl"); exit(1);
        }

        printf("메일수 (%ld)\n", m_stat.msg_qnum);
        printf("1. 받은 메일 읽기\n2. 새로고침\n3. 종료\n>>> ");
        scanf("%d", &mode);                                         // 1, 2, 3 입력받음

        if (mode == 3)                                              // 종료
            break;
        else if (mode == 1){                                        // 메일 읽기
            for (int i = 0; i < m_stat.msg_qnum; i++) {             // 메시지 개수만큼 반복
                msgrcv(msgid, &inmsg, 80, 0, 0);                    // 메시지를 읽어 출력
                printf("-> %s\n", inmsg.mtext);
            }              
            printf("    [보낸이 PID: %d]\n\n", m_stat.msg_lspid);   // 메시지 출력후 송신자 pid출력
        }
        else if(mode == 2) {                                        // 새로고침
            printf("\n");
            continue;                                               // 다음 반복으로
        } 
    }
    msgctl(msgid, IPC_RMID, &m_stat);

    return 0;
}