#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main() {
	printf("원래 프로세스: %d\n", getpid());
	sleep(1);
	execl("/bin/sh", "sh", NULL);
	exit(0);
}
