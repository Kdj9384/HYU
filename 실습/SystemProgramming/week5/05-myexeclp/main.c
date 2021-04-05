#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
int main(int argc, char *argv[])
{
    extern char **environ;
    char **ptr;
    for (ptr = environ; *ptr != NULL; ptr++)
    {
        printf("%s\n", *ptr);
    }
    printf("getenv : %s\n", getenv("PATH"));
    printf("%s\n", argv[0]);

    pid_t pid = fork();
    if (pid == 0)
    {
        char *env[] = {"PATH=hhihihi/hih", NULL};
        execle("main2", "main2", (char *)0, env);
    }
    return 0;
}