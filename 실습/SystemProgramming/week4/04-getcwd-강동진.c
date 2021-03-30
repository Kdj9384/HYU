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
void creatEnv();

/**
 * 
 * @brief 
 *  우분투 환경에서 작성되었습니다.
 * 
 * 파일을 복사하여 사본을 생성한다. 
 * @param buf   경로명을 저장할 버퍼 변수.
 * @param size  버퍼 변수의 크기
 * @return 버퍼 변수의 시작 주소, 즉 buf
 */
char *mygetcwd(char *buf, size_t size)
{
  int pipe_fd[2];
  int child_pid;

  // pipe 생성
  if (pipe(pipe_fd) < 0)
  {
    perror("pipe error");
  }

  // 자식 프로세스 생성 ( 작업 디렉토리의 변경이 부모에 영향을 주지 않기 위해)
  child_pid = fork();

  if (child_pid < 0)
  {
    perror("fork error");
  }

  // 자식 프로세스
  else if (child_pid == 0)
  {
    // 사용하지 않는 파이프를 닫아준다.
    close(pipe_fd[0]);

    pid_t pid;
    int status;

    ino_t currentIno = 0;
    ino_t temp = 1;

    char *array[255];
    int arrayCNT = 0;

    *buf = '\0';

    while (1)
    {
      // 현재 디렉토리를 연다.
      DIR *dirp = opendir(".");
      struct dirent *dp;
      // 반복문을 통해 dir entry를 탐색
      while (1)
      {
        // entry 읽기 성공
        if ((dp = readdir(dirp)) == NULL)
        {
          break;
        }

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

          arrayCNT++;
        }
      }
      // 부모 inode 와 자식 inode 가 같은 경우
      if (temp == currentIno)
      {
        closedir(dirp);
        break;
      }
      currentIno = temp;

      // 현재 열린 dir stream을 닫아준다.
      closedir(dirp);

      // 작업 디렉토리 이동
      chdir("..");
    }

    // 결과 문자열 만들기
    for (int i = arrayCNT - 3; i >= 0; i--)
    {
      strcat(buf, "/");
      strcat(buf, array[i]);
      free(array[i]);
    }
    strcat(buf, "\0");

    // 완성된 문자열을 파이프에 써준다.
    write(pipe_fd[1], buf, size);

    exit(0);
  }

  // 부모 프로세스
  else
  {
    wait(NULL);

    // 파이프에 저장된 문자열을 읽어온다.
    read(pipe_fd[0], buf, size);

    return buf;
  }
}

int main(void)
{
  pid_t pid;
  int status;
  char buf[255];

  creatEnv();
  chdir("dir/sub");

  printf("original func: %s\n", getcwd(NULL, 0));
  printf("mygetcwd func: %s\n", mygetcwd(buf, 255));

  return 0;
}

void creatEnv()
{
  mkdir("dir", 0755);
  mkdir("dir/sub", 0755);
  mkdir("dir/sub2", 0);

  creat("dir/a", 0755);
  creat("dir/b", 0755);
  creat("dir/sub/x", 0755);
  symlink("dir/a", "dir/sl");
  symlink("dir/x", "dir/dsl");
}