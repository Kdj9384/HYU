#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int testF(char *args[])
{
    printf("%c\n", *args[0]);
    *args[0] = 'h';
    return 0;
}

int main(void)
{

    char *args[] = {"ls", "hi", "-l"};
    if (*args[0])

        testF(args);
    printf("%c\n", *args[0]);

    // pid_t child = fork();

    // char *const list[] = {"ls", "&", NULL};

    // if (child == 0)
    // {
    //     for (int i = 0; i < 5; i++)
    //     {
    //         sleep(3);
    //     }
    // }
    // else
    // {
    //     wait(NULL);
    //     printf("parent finish");
    // }
}
