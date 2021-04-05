#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
	int pid;
	int status;
	int spid;
	if ((pid = fork()) == 0) {
		sleep(5);
		printf("I will be back %d\n", getpid());
		return 1;
	} else if (pid > 0) {
		printf("I'm parent %d\n", getpid());
		printf("Press any key and wait\n");
		getchar();
		// 자식프로세스를 wait 한다.
		// 자식프로세스의 종료 상태는 status를 통해 받아온다.
		spid = wait(&status);
		printf("자식프로세스 wait 성공\n");
		// 자식프로세스의PID, 리턴 값, 종료 상태(정상 혹은 비정상종료)를 얻어옴
		printf("PID: %d\n", spid);
		printf("Exit Value : %d\n", WIFEXITED(status));
		printf("Exit Stat : %d\n", WEXITSTATUS(status));
	} else {
		perror("fork error :");
	}
}
