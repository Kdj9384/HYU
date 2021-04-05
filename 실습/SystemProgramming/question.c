#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char *env[] = {"USER=unknow", "PATH=/tmp", NULL};

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork error\n");
        exit(1);
    }

    // 자식
    else if (pid == 0)
    {
        if (execle("~/bin/echoall", "echoall", "arg1", "arg2", '\0', env) < 0)
        {
            fprintf(stderr, "execle error \n");
            exit(1);
        }

        // 에러시에만 종료(exit(1)) 성공적으로 수행시 if else 문 이후로 넘어감
        printf("success exe");
    }
    // 부모
    else
    {
    }

    // 자식, 부모 둘 다 접근 가능.
    if (waitpid(pid, NULL, 0) < 0)
    {
        perror("wait error\n");
        exit(1);
    }

    // 위에 자식 프로세스에서 종료되지 않았다면 실행 가능.
    else if (pid == 0)
    {
        if (execlp("echoall", "echoall", "only 1 arg", '\0') < 0)
        {
            fprintf(stderr, "execlp error\n");
            exit(1);
        }
    }

    exit(0);
}
