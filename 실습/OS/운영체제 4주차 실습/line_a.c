#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int value = 5;

int main()
{
	pid_t pid;

	int fd[2];

	pipe(fd);

	pid = fork();

	if (pid == 0)
	{
		close(fd[0]);
		value += 15;

		write(fd[1], &value, sizeof(value));
		return 0;
	}
	else if (pid > 0)
	{
		wait(NULL);

		close(fd[1]);
		read(fd[0], &value, 4);
		printf("PARENT: value = %d\n", value); /* LINE A */
		return 0;
	}
}
