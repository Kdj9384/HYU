#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

int main()
{
    extern char **environ;
    char **ptr;
    for (ptr = environ; *ptr != NULL; ptr++)
    {
        printf("%s\n", *ptr);
    }

    printf("getenv : %s\n", getenv("PATH"));
}
