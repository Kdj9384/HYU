#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#define MAXARGS 31
#define MAXFILENAME 1024
#define MAXPATHSTR 2048

extern char **environ;
/**
 * 첫 번째 인자인 file 프로그램을 뒤에 따르는 인자들을 이용해 실행한다.
 * @param file	실행할 프로그램의 이름
 * @param args	stdarg(3)을 통해 처리할 인자들
 * @return 실패했을 경우 -1, 성공 시 반환 값 없음
 */
int myexeclp(const char *file, const char *args, ...)
{
  char *temp;
  char *argv[MAXARGS];

  // 가변 변수 포인터
  va_list ap;
  // va_start(ap, 마지막 고정 파라미터);
  va_start(ap, args);
  // argv의 첫 인자는 프로그램 이름
  argv[0] = args;
  // 가변 변수를 argv[]에 저장
  int cnt = 1;
  do
  {
    temp = va_arg(ap, char *);
    argv[cnt] = temp;
    cnt++;

  } while (temp != (char *)0);
  va_end(ap);

  char path[MAXPATHSTR];
  char *str = getenv("PATH");
  char *path_tok[MAXARGS];
  int path_cnt = 0;

  // 하나의 string으로 받아온 PATH를 ':' 을 기준으로 나누어 path_tok[]에 저장한다.
  char *strtok_p = strtok(str, ":");
  while (strtok_p != NULL)
  {
    // 환경변수에 파일이름을 붙여서 경로로 만들어준다.
    sprintf(path, "%s/%s", strtok_p, file);

    path_tok[path_cnt] = (char *)malloc(sizeof(path));
    strcpy(path_tok[path_cnt], path);

    path_cnt++;
    strtok_p = strtok(NULL, ":");
  }

  // 저장한 path_tok[]를 뒤에서부터 실행시켜준다.
  while (path_cnt >= 0)
  {
    char *env[] = {getenv("PATH"), NULL};
    if (execve(path_tok[path_cnt - 1], argv, env) < 0)
    {
      // perror("execve error");
    }
    path_cnt--;
  }

  // 실패시 -1 리턴.
  return -1;
}

int main(void)
{
  char path[MAXPATHSTR];
  // 현재 디렉토리를 PATH에 추가.
  sprintf(path, "PATH=%s:%s", getcwd(NULL, 0), getenv("PATH"));
  putenv(path);
  // prepare the executable file named "hello"
  system("gcc -o hello hello.c");
  // execlp("hello", "hello", "-a", "-b", "-c", (char *)0);
  myexeclp("hello", "hello", "-a", "-b", "-c", (char *)0);

  return 0;
}