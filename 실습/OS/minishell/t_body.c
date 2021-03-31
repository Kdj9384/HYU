#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */
int main(void)
{

    // 전위 표기법을 사용하여 들어온 문자열을 처리한다음 진행. 중요도는 |  >< 순이고 나머지는 숫자취급함.
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;           /* flag to determine when to exit program */

    // while (should_run)
    {
        printf("osh> ");
        fflush(stdout);

        // 명령어 파싱 (파이프 유무, & 유무, 명령어 분기 처리)
        char input[MAX_LINE];
        if (fgets(input, MAX_LINE, stdin) == NULL)
        {
            perror("fgets error");
        } // input 에 "~~~~NULL\n" 로 입력이 받아짐.

        // 공백 단위로 split
        char *strtok_p = strtok(input, " ");

        int args_length = 0;
        while (strtok_p != NULL)
        {
            args[args_length] = strtok_p;
            // printf("%d : %s\n", args_length, args[args_length]);
            strtok_p = strtok(NULL, " ");
            args_length++;
        }
        args[args_length] = NULL;

        // 개행 문자 제거
        int leng = strlen(args[args_length - 1]);
        *(args[args_length - 1] + (leng - 1)) = '\0';
    }
    return 0;
}