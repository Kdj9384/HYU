#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int i;
    struct stat buf;
    char *ptr;

    lstat(argv[1], &buf);

    printf("%d", S_ISREG(buf.st_mode));
}