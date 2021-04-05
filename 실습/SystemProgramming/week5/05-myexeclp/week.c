#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#define MAXARGS 31
#define MAXFILENAME 1024
#define MAXPATHSTR 2048

extern char **environ;

int myexeclp(const char *file, const char *args, ...)
{
    // 가변 변수 포인터
    va_list ap;
    // va_start(ap, 마지막 고정 파라미터);
    va_start(ap, args);

    char *temp;
    char *argv[MAXARGS];

    // argv의 첫 인자는 프로그램 이름
    argv[0] = args;

    // 가변 변수를 argv[]에 저장
    int cnt = 1;
    do
    {
        temp = va_arg(ap, char *);
        // printf("%s\n", temp);
        argv[cnt] = temp;
        cnt++;

    } while (temp != (char *)0);
    va_end(ap);

    char path[MAXPATHSTR];
    char *str = getenv("PATH");
    char *path_tok[MAXARGS];
    int path_cnt = 0;
    // printf("ENRIE PATH -> %s\n", str);

    // 하나의 string으로 받아온 PATH를 ':' 을 기준으로 나누어 path_tok[]에 저장한다.
    char *strtok_p = strtok(str, ":");
    while (strtok_p != NULL)
    {
        sprintf(path, "%s/%s", strtok_p, file);
        // printf("path: %s\n", path);

        path_tok[path_cnt] = (char *)malloc(sizeof(path));
        strcpy(path_tok[path_cnt], path);

        path_cnt++;
        strtok_p = strtok(NULL, ":");
    }

    // 저장한 path_tok[]를 뒤에서부터 실행시켜준다.
    while (path_cnt >= 0)
    {
        // printf("path_tok : %s\n", path_tok[path_cnt - 1]);
        char *env[] = {getenv("PATH"), NULL};

        if (execve(path_tok[path_cnt - 1], argv, env) < 0)
        {
            // perror("execve error");
        }

        path_cnt--;
    }

    // 실패시 -1 리턴.
    return -1;
}
int main()
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    {
        perror("error");
    }
    else if (pid == 0)
    {
        char path[MAXPATHSTR];
        sprintf(path, "PATH=%s:%s", getcwd(NULL, 0), getenv("PATH"));
        putenv(path);
        myexeclp("hello", "hello", "-a", "-b", (char *)0);

        // 환경변수가 추가되면 execvp 정상 실행됨.
        // putenv(path);
        // execvp("hello", vector);
        return -1;
    }
    else
    {
        waitpid(-1, &status, 0);
        exit(1);
    }
}