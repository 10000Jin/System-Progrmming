#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int rfd, wfd, n;
	char buf[10];

	rfd = open(argv[1], O_RDONLY);
	if (rfd == -1){
		perror("Open tmp.txt");
		exit(1);
	}

	wfd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (wfd == -1) {
		perror("Open res.txt");
		exit(1);
	}

	while ((n = read(rfd, buf, 6)) > 0)
		if (write(wfd, buf, n) != n) perror("Write");

	if (n == -1) perror("Read");

	close(rfd);
	close(wfd);

	return 0;
}
