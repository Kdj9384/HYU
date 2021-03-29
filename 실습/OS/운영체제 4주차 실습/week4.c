#include <stdio.h>
#include <unistd.h>

int main()
{
    int i;

    char *buff = "hi\n";
    char *buff2 = "hi2\n";

    write(1, buff, sizeof(buff));

    pid_t pid = fork();

    if (pid == 0)
    {
        execv("/bin/ls", NULL);
    }
    else
    {
        printf("hello");
    }

    // sleep(120);
    return 0;
}

// execv 안됨
// 포인터를 fork 하는 경우 -> 복사하면서 주소값이 바뀜
// pipe 는 단방향이기 때문에 2개의 element중 하나를 반드시 닫아줘야함.
// pipe 에서