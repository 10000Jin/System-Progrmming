#include <sys/times.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	struct tms mytms;				// running time 정보를 저장할 구조체
	clock_t t1, t2;

	int rfd, wfd, n, fee;				// 원본파일과 백업파일의 file descriptor변수 선언
	double rtime, utime, stime;			// real, user, system running time변수 선언
	char buf[16];					// 읽은 내용이 저장된 배열

	if ((t1 = times(&mytms)) == -1) { perror("times 1"); exit(1); } // 백업전 시간 저장

	sleep(5);					// 5초간 프로세스 멈춤

	rfd = open(argv[1], O_RDONLY);			// 원본파일 읽기 전용으로 엶
	if (rfd == -1){
		perror("File open .PDF");
		exit(1);
	}
	
	wfd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644); // 백업파일 쓰기전용으로 엶
	if (wfd == -1) {
		perror("File open .bak");
		exit(1);
	}

	while ((n = read(rfd, buf, 16)) > 0)			// 원본파일 내용 16만큼 읽어 buf에 저장
		if (write(wfd, buf, n) != n) perror("Write");	// 백업파일에 buf의 내용 씀

	if (n == -1) perror("Read");


	close(rfd);					// 파일 닫음
	close(wfd);
	
	if ((t2 = times(&mytms)) == -1) { perror("times 2"); exit(1); } // 백업후 시간 저장

	rtime = (double)(t2-t1) / sysconf(_SC_CLK_TCK);		// t1과 t2를 빼서 real time구함
	utime = (double)mytms.tms_utime / sysconf(_SC_CLK_TCK); // user time 구함
	stime = (double)mytms.tms_stime / sysconf(_SC_CLK_TCK);	// system time 구함

	fee = utime * 100 + stime * 200;			// 금액 계산

	printf("Real time : %.2lf sec\n", rtime);		// 결과 출력
	printf("User time : %.2lf sec\n", utime);
	printf("System time : %.2lf sec\n", stime);

	printf("사용료는 %d원 입니다.\n", fee);

	return 0;
}
