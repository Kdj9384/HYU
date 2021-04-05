#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char **argv) {
	int pid;
	if ((pid = fork()) > 0) {
		// getpid() 현재 프로세스의 프로세스ID를 되돌려 줌
		printf("부모 프로세스 %d : %d\n", getpid(), pid);
		return 0;
	} else if (pid == 0) {
		printf("자식 프로세스 %d\n", getpid());
		return 0;
	} else {
		perror("fork error : ");
		exit(0);
	}
	return 0;
}
