#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main() {
	int pid = 0;
	if ((pid = fork()) == 0) {
		sleep(1);
		printf("My pid %d\n", getpid());
		printf("My Parent pid %d\n", getppid());
	} else if (pid > 0) {
		printf("Parent pid %d\n", getpid());
		sleep(5);
	} else {
		perror("fork error :");
		exit(0);
	}
	return 0;
}
