#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <errno.h>
#include <string.h>
#include <dirent.h>

/**
 * @brief 
 * 
 * dirp = opendir("./") -> dp = readdir(dirp) => dp->ino 를 구할 수 있음
 * cd ..
 * dirp = opendir("./") -> dp = readdir(dirp)
    1. 오류 확인
    2. "." inode number 를 temp 에 저장
    3. ".." inode number 를 temp2 에 저장
    4. "." 와 ".." inode number 비교
    5. currentIno 와 같은 inode number 를 가지는 파일의 이름을 버퍼에 저장
 * 
 * @return int 
 */

int main(void)
{
    pid_t pid;
    int status;
    char buf[255];

    ino_t currentIno = 0;
    ino_t temp = 1;

    char *pathTemp = "/";

    char *array[255];

    int arrayCNT = 0;

    while (3)
    {
        // 현재 디렉토리를 연다.
        DIR *dirp = opendir(".");
        struct dirent *dp;
        // 반복문을 통해 dir entry를 탐색
        while (1)
        {
            printf("currentIno : %llu  /  temp : %llu\n", currentIno, temp);
            // entry 읽기 성공
            if ((dp = readdir(dirp)) == NULL)
            {
                printf("\n readdir error \n");
                break;
            }

            printf("읽기 성공");

            // 지금 가리키는 파일이름이 "." 이면, 현재 디렉토리의 inode를 temp에 저장
            if (strcmp(dp->d_name, ".") == 0)
            {
                temp = dp->d_ino;
            }

            // currentIno 와 같은 파일이 있다면, 그 이름을 저장함.
            if (dp->d_ino == currentIno)
            {
                array[arrayCNT] = malloc(sizeof(char) * strlen(dp->d_name) + 1);
                strcpy(array[arrayCNT], dp->d_name);

                // pathTemp = dp->d_name;
                printf("\n path : %s\n", array[arrayCNT]);
                arrayCNT++;
            }

            printf("%llu : %s\n", dp->d_ino, dp->d_name);
        }
        printf("current dir finished\n");
        // 부모 inode 와 자식 inode 가 같은 경우
        if (temp == currentIno)
        {
            printf("current dir is root\n");
            closedir(dirp);
            break;
        }
        currentIno = temp;
        // 현재 디렉토리 inode number 알아냄.
        // sleep(1);
        closedir(dirp);
        chdir("..");
    }

    chdir("/Users/dongjin/Documents/3학년 1학기/실습/SystemProgramming/week4/");
    // buf 에 문자열 복사

    // 결과 출력

    printf("\n");

    for (int i = arrayCNT - 2; i >= 0; i--)
    {
        strcat(buf, "/");
        strcat(buf, array[i]);
        free(array[i]);
    }

    strcat(buf, "/\0");

    
    printf("last : %s\n", buf);

    return 0;
}
