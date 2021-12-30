#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdio.h>

extern int errno;

int checksize(char* a, char* b){			// 파일 사이즈 검사 함수
	struct stat buf, buf2;				// file, file2의 정보를 저장할 구조체 선언

	stat(a, &buf);					// file의 파일 정보를 buf에 저장
	stat(b, &buf2);					// file2의 파일 정보를 buf2에 저장

	if ((int)buf.st_size > (int)buf2.st_size){	// file의 사이즈가 file2보다 크면
		printf("file.txt is bigger\n");		// 문구 출력
		return 1;				// 1 반환
	}
	
	else if ((int)buf.st_size < (int)buf2.st_size){	// file2의 사이즈가 file보다 크면
		printf("file2.txt is bigger\n");
		return 2;				// 2반환
	}
	else {						// 같으면
		printf("same size\n");
		return 0;				// 0반환
	}

}


int main(int argc, char* argv[]) {
	int chkSize = 0;				// 파일 사이즈 검사하는 함수의 반환값이 저장되는 변수
	int per;

	if (argc != 3)					// 인자가 3개가 들어오지 않으면 안내문구 출력
		printf("file.txt와 file2.txt를 정확히 설정해주십시오.");

	chkSize = checksize(argv[1], argv[2]);		// 두 파일의 사이즈 검사 함수 호출


	if (chkSize == 1){							// 1이면(file이 더큼)
		if (access(argv[1], F_OK) == -1 && errno == ENOENT)		// 파일 존재하지 않으면 
			printf("%s : File not exist.\n", argv[1]);		// 문구 출력

		per = access(argv[1], R_OK);					// 읽기 권한
		if (per == 0)							// 읽기 권한이 있으면
			printf("%s : Read permission is permitted.\n", argv[1]);
		else if (per == -1 && errno == EACCES)				// 읽기 권한이 없으면
			printf("%s : Read permission is not permitted.\n", argv[1]);
	}
	
	else if(chkSize == 2){							// 2이면(file2가 더큼)  
		if (access(argv[2], F_OK) == -1 && errno == ENOENT)
			printf("%s : File not exist.\n", argv[2]);

		per = access(argv[2], R_OK);
		if (per == 0)
			printf("%s : Read permission is permitted.\n", argv[2]);
		else if (per == -1 && errno == EACCES)
			printf("%s : Read permission is not permitted.\n", argv[2]);
	}

	else									// 0이면(사이즈 같음)
		return 0;							// 종료
	
		

	return 0;
}
