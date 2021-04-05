#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int glob = 6;
char buf[] = "a write to stdout\n";
int main(void) {
	int var, size;
	pid_t pid;
	var = 88;
	size = sizeof(buf) - 1;
	if (write(STDOUT_FILENO, buf, size) != size) {
		perror("write error");
		exit(1);
	}
	/* we don't flush stdout */
	printf("before fork\n");
	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) { /* child */
		glob++; /* modify variables */
		var++;
	} else {
		sleep(2); /* parent */
	}
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}

