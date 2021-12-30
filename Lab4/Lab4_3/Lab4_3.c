#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
	uid_t uid, euid, new_euid;				// uid, euid new_euid 변수 선언
	FILE *rfp, *wfp;					// db 파일과 txt 파일의 file pointer변수 선언
	
	int in_stuNum = 0;					// 입력받은 학번 변수 0초기화
	int data_stuNum = 0;					// 데이터의 학번 변수 0초기화
	int check = 0;						// 일치하는 학번이 있는지에 대한 변수
	char grade[5];						// 성적을 저장할 배열


	while (*argv[1]) {					// 입력받은 학번을 정수형으로 변환
		in_stuNum = in_stuNum * 10 + (int)(*argv[1] - 48);	// in_stuNum에 저장
		argv[1]++;
	}


	uid = getuid();						// uid 저장
	euid = geteuid();					// euid 저장
	printf("uid = %d, euid = %d\n", (int)uid, (int)euid);	// uid와 euid 출력
	
	if ((rfp = fopen("/home/professor/grade.db", "r")) == NULL) {	// db파일 읽기 전용으로 엶
		perror("fopen: grade.db");
		exit(1);
	}


	seteuid(uid);						// txt파일 열기 전 euid를 현재 uid로 설정
	new_euid = geteuid();					// euid를 new_euid에 저장

	if ((wfp = fopen(argv[2], "w")) == NULL) {		// txt파일을 쓰기 전용으로 엶.
		perror("fopen: myGrade.txt");
		exit(1);
	}
	
	while (!feof(rfp)) {					// db파일의 끝까지 반복
		fscanf(rfp, "%d %s", &data_stuNum, grade);	// 학번과 학점을 읽어 각 변수에 저장
		if (in_stuNum == data_stuNum) {			// 입력 학번과 db의 학번이 같으면
			printf("my grade = %s\n", grade);
			fprintf(wfp, "%s\n", grade);		// 학점과 uid, euid 출력
			printf("uid = %d, euid = %d\n", (int)uid, (int)new_euid);
			printf("Write the grade to %s\n", argv[2]);
			check = 1;				// 일치하는 것이 있으므로 check 1저장
			break;
		}
	}

	if (check == 0)						// check가 0이면 일치하는 학번이 없음
		printf("입력한 학번에 해당하는 데이터가 없습니다.\n");


	fclose(rfp);						// 파일 닫아줌
	fclose(wfp);

	return 0;
}


	
