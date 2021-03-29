/**
 * @file body.c
 * @author 2017012642 강동진
 * @brief 
 *  [운영체제론] 1차 프로젝트 과제
 *  미니 쉘 구현하기.
 * 
 *  -- 기능 --
 *  명령어 or 명령어 &
 *  명령어 > 파일 or 명령어 > 파일 &
 *  명령어 < 파일 or 명령어 < 파일 &
 *  명령어1 | 명령어2 or 명령어1 | 명령어2 & 
 *  (단 명령어는 옵션을 포함함.)
 * 
 * @version 0.1
 * @date 2021-03-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */

// 주어진 명령어를 실행하는 함수로, <, > 를 처리할 수 있다.
int exeInstruction(int RD_Lflag, int RD_Rflag, int BG_flag, char *argp[]);

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;           /* flag to determine when to exit program */

    while (should_run)
    {
        printf("osh> ");
        fflush(stdout);

        // 명령어 파싱 (파이프 유무, & 유무, 명령어 분기 처리)
        char input[MAX_LINE];
        if (fgets(input, MAX_LINE, stdin) == NULL)
        {
            perror("fgets error");
        }

        // 공백 단위로 split
        char *strtok_p = strtok(input, " ");
        int args_length = 0;
        while (strtok_p != NULL)
        {
            args[args_length] = strtok_p;
            strtok_p = strtok(NULL, " ");
            args_length++;
        }

        // execvp 의 args의 마지막 인자로 NULL 이 필요하기 때문에 삽입해줌.
        args[args_length] = NULL;

        // 개행 문자 제거
        int leng = strlen(args[args_length - 1]);
        *(args[args_length - 1] + (leng - 1)) = '\0';

        // 명령어 탐색
        // pipe_flag : args에서 pipe의 위치
        int pipe_flag = 0;
        int BG_flag = 0;
        int RD_Lflag = 0;
        int RD_Rflag = 0;
        for (int i = 0; i < args_length; i++)
        {
            if (*args[i] == '|')
            {
                pipe_flag = i;
            }

            if (*args[i] == '&')
            {
                BG_flag = i;
            }

            if (*args[i] == '>' && RD_Rflag == 0)
            {
                RD_Rflag = i;
            }

            if (*args[i] == '<' && RD_Lflag == 0)
            {
                RD_Lflag = i;
            }
        }

        char *args2[MAX_LINE / 2 + 1];
        int RD_Lflag2 = 0;
        int RD_Rflag2 = 0;
        int BG_flag2 = 0;

        // pipe가 있는 경우, pipe이후의 명령을 처리하기 위한 전처리 과정
        if (pipe_flag)
        {

            args[pipe_flag] = NULL;

            for (int i = 1; i < (MAX_LINE / 2 + 1); i++)
            {
                if (args[pipe_flag + i] == NULL)
                {
                    args2[i - 1] = NULL;
                    break;
                }
                else
                {

                    args2[i - 1] = args[pipe_flag + i];
                }
            }

            for (int i = 0; args2[i] != NULL; i++)
            {

                if (*args2[i] == '>')
                {
                    RD_Rflag2 = i;
                }

                if (*args2[i] == '<')
                {
                    RD_Lflag2 = i;
                }

                if (*args2[i] == '&')
                {
                    BG_flag2 = i;
                }
            }
        }

        // & 제거
        if (BG_flag)
        {
            args[BG_flag] = NULL;
            args2[BG_flag2] = NULL;
        }

        // exit 검사
        if (strcmp(args[0], "exit") == 0)
        {
            // 같음
            should_run = 0;
            continue;
        }

        // --------------------------------------------------------------------------------
        // pipe 없을 때

        if (pipe_flag == 0)
        {
            exeInstruction(RD_Lflag, RD_Rflag, BG_flag, args);
        }

        // --------------------------------------------------------------------------------
        // pipe 있을 때

        else
        {
            pid_t child_pid = fork();

            if (child_pid < 0)
            {
                perror("pipe fork error");
            }
            else if (child_pid == 0)
            {
                // 1 : write
                // 0 : read
                int pipe_fd[2];

                if (pipe(pipe_fd) == -1)
                {
                    perror("pipe error");
                }

                pid_t child2_pid = fork();
                if (child2_pid < 0)
                {
                    perror("pipe fork error");
                }
                else if (child2_pid == 0)
                {
                    close(pipe_fd[0]);
                    dup2(pipe_fd[1], 1);
                    exeInstruction(RD_Lflag, RD_Rflag, 0, args);
                    return 0;
                }
                else
                {
                    close(pipe_fd[1]);
                    dup2(pipe_fd[0], 0);
                    exeInstruction(RD_Lflag2, RD_Rflag2, 0, args2);
                    return 0;
                }

                return 0;
            }
            else
            {
                // & 를 사용한 경우
                if (BG_flag)
                {
                    // not wait
                    int status;
                    int process_cnt = 1;
                    int retval = waitpid(-1, &status, WNOHANG);
                    if (retval == 0)
                    {
                        // 백그라운드로 실행됨을 알려주기 위함.
                        printf("[%d] %d\n", process_cnt, child_pid);
                    }
                }
                // & 를 사용하지 않는 경우
                else
                {
                    // wait
                    int status;
                    int retval = waitpid(-1, &status, 0);
                }
            }
        }

        // 반복
    }
    return 0;
}

// ================================================================================

int exeInstruction(int RD_Lflag, int RD_Rflag, int BG_flag, char *argp[])
{
    pid_t child_pid = fork();

    if (child_pid < 0)
    {
        perror("child_pid error");
        return -1;
    }
    else if (child_pid == 0)
    {
        // '<' 가 사용된 경우
        if (RD_Lflag)
        {
            int fd;
            // 파일이 없는 경우 에러
            fd = open(argp[RD_Lflag + 1], O_RDONLY | O_NONBLOCK, 0600);
            if (fd < 0)
            {
                perror("no file");
            }
            else
            {
                // 표준 입력을 파일로 바꿔준 다음, execvp가 처리할 수 있게 '<'를 null 로 바꿔준다.
                dup2(fd, 0);
                argp[RD_Lflag] = NULL;
                execvp(argp[0], argp);
                perror("execvp Error Occured");
            }
            close(fd);
            return 0;
        }
        // '>' 를 사용한 경우.
        else if (RD_Rflag)
        {
            int fd;
            // 파일이 없는 경우 생성
            fd = open(argp[RD_Rflag + 1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
            dup2(fd, 1);

            argp[RD_Rflag] = NULL;
            execvp(argp[0], argp);
            perror("execvp Error Occured");
            close(fd);
            return 0;
        }
        else
        {
            // '<' 와 '>' 가 동시에 사용된 경우.
            if (RD_Lflag && RD_Rflag)
            {
                perror("too many redirections ");
            }
            // 명령어만 실행하는 경우
            else
            {
                execvp(argp[0], argp);
                return 0;
            }
        }
    }
    else
    {
        // '&' 를 사용한 경우, WNOHANG 을 사용하여 background process 구현
        if (BG_flag)
        {
            // not wait
            int status;
            int process_cnt = 1;
            int retval = waitpid(child_pid, &status, WNOHANG);
            if (retval == 0)
            {
                printf("[%d] %d\n", process_cnt, child_pid);
            }
        }
        // '&' 를 사용하지 않은 경우
        else
        {
            // wait
            int status;
            int retval = waitpid(child_pid, &status, 0);
        }
    }
    return 0;
}