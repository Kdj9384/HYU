#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int fd, fd_in;
    mode_t old_mask, new_mask;

    old_mask = umask(022);
    fd = open("test_out", O_CREAT | O_WRONLY, 0666);
    // fd_in = open("test_in", O_CREAT | O_RDONLY, 0666);

    // dup2(fd, 1);
    // dup2(fd_in, 0);

    printf("old_mask %o\n", old_mask);

    new_mask = umask(old_mask);
    printf("new mask %o\n", new_mask);

    // printf("this is stdin %s", argv[1]);

    close(fd);
    // close(fd_in);
}