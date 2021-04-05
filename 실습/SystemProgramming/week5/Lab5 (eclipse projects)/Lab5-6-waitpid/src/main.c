#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
int main() {
	int pid;
	int status;
	if ((pid = fork()) < 0) {
		perror("fork error : ");
		exit(0);
	} else if (pid == 0) {
		printf("I'm Child\n");
		sleep(10);
		return 2;
	}

	else {
		printf("Parent: wait (%d)\n", pid);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) {
			printf("정상 종료\n");
			printf("리턴 값 %d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			printf("신호 받았음\n");
			printf("신호 번호 %d\n", WTERMSIG(status));
		}
	}
	exit(0);
}
