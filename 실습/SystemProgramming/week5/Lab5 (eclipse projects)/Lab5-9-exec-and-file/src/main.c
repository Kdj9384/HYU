/* 빌드 시 openfexec 제외하기
 * 프로젝트 이름 우클릭 > Properties > C/C++ General
 * > Paths and Symbols > Source Location
 * > /"프로젝트이름"/src 선택 후 Edit Filter
 * > Add > Browse > openfexec.c 선택 후 OK
 * > OK를 반복 선택하여 창 닫기 > Apply and Close
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int fdes, pid, cflag, excode;
	long pos;
	char sfdes[10];
	system("gcc ./src/openfexec.c -o openfexec");
	/* open its srcfile */
	fdes = open("./src/main.c", O_RDONLY);
	sprintf(sfdes, "%d", fdes);
	/* print file descriptor */
	printf("fdes = %d\n", fdes);
	/* 20 is an arbitrary number */
	pos = lseek(fdes, 20l, SEEK_SET);
	printf("Current position before fork() is %ld\n", pos);
	if (!fork()) { /* child */
		/* 40 is also arbitrary */
		pos = lseek(fdes, 40l, SEEK_SET);
		printf("Current position in child after fork() is %ld\n", pos);
	} else { /* parent */
		/* wait for 1st child to terminate */
		wait((int*) 0);
		/* to get pos in file */
		pos = lseek(fdes, 0l, SEEK_CUR);
		printf("Current position in parent after fork() is %ld\n", pos);
		/* fork again --child process */
		if (!fork()) {
			/* overlay with another program */
			execl("./openfexec", "openfexec", sfdes, (char*) 0);
			printf("Itis an error to print this line out\n");
		}
		/* parent --no need for else */
		wait(&excode);
		/* exit code is needed */
		pos = lseek(fdes, 0l, SEEK_CUR);
		printf("Current pos in parent after exec() is %ld\n", pos);
		printf("Exit code of a child = %d\n", WEXITSTATUS(excode));
		/* 3rd arg can be any in there */
		cflag = fcntl(fdes, F_GETFD, 0);
		printf("close-on-exec flag = %d\n", cflag);
		/* set close-on-exec flag */
		fcntl(fdes, F_SETFD, 1); // 1 = set
				/* fork again --parent process */
		if ((pid = fork()) != 0) {
			/* wait for a specific child to end */
			waitpid(pid, &excode, 0);
			printf("Exit code of a specific child = %d\n", WEXITSTATUS(excode));
			/* parent terminates */
			exit(0);
		}
		/* child executes another program */
		execl("./openfexec", "openfexec", sfdes, (char*) 0);
		printf("It is an error to print this line out\n");
	}
}

