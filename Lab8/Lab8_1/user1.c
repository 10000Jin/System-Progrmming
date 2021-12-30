#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHECK_MMAP_SUCCESS(_addr)	\
if (_addr == MAP_FAILED) 		\
{					\
	perror("mmap");			\
	exit(1);			\
}

int getstrlen(char* str);

int main(int argc, char* argv[]) {
	int fd, i = 1;
	char message[20];
	char* send;
	caddr_t addr;
	
	fd = open("tmp.txt", O_CREAT | O_RDWR | O_EXCL | O_APPEND, 0644);
	if (fd == -1) {
		perror("tmp.txt");
		exit(1);
	}
	ftruncate(fd, 100);

	addr = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
	CHECK_MMAP_SUCCESS(addr);
	close(fd);
	
	printf("[발신] : ");
	scanf("%s", message);
	//getstrlen(message);

	send = message;
	while(*send){
		addr[i++] = *send;
		send++;
	}

	addr[0] = 1;
	addr[i] = '\0';	
	//printf("%s\n", addr);

	sleep(1);
	while(1) {
		if (addr[0] == 1){
			printf("[수신] : ");
   
     			//send = message;
	           	for (i; addr[i] != '\0'; i++){
				printf("%c", addr[i]);
			}	

			printf("\n");
			addr[0] = 0;
			break;
		
		}	
	}

	//close(fd);
	return 0;


}

int getstrlen(char* str) {
	int len = 0;
	for (int i = 0; str[i] != '\0'; i++)
		len++;
	printf("len: %d\n", len);

	return len;
}
