#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void other(int fd);

void main()
{
    int fd;
    char buff[] = "123";
    fd = open("file.hole", O_RDONLY);

    int value = fcntl(fd, F_GETFD);

    value |= O_APPEND;

    fcntl(fd, F_SETFD, value);

    value = fcntl(fd, F_GETFD);

    if ((value & O_WRONLY) == O_WRONLY)
    {
        write(1, buff, 3);
    }

    // other(fd);
    // printf(" main fd %d", fd);
    // write(1, buff, 3);
}

void other(int fd)
{
    printf(" other fd %d", fd);
}