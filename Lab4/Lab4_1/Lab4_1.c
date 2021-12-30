#include <stdio.h>
#include <stdlib.h>
#include <grp.h>

int main(void) {
	struct group *grp; 				// 그룹의 정보를 가지고 있는 구조체를를 가르킬 포인터 변수
	int n;						// 그룹의 멤버를 출력하기 위한 변수

	while ((grp = getgrent()) != NULL) {		// 그룹의 정보 위치를 grp에 저장 (끝날 때까지)
		printf("Group Name : %s\n", grp->gr_name);	// 그룸명 출력
		printf("GID : %d\n", (int)grp->gr_gid);		// 그룹 GID출력
	
		n = 0;
		printf("Members : ");
		while (grp->gr_mem[n] != NULL)			// 모든 그룹 멤버명에 대해 출력
			printf("%s ", grp->gr_mem[n++]);
		printf("\n");
	}

	return 0;
}
