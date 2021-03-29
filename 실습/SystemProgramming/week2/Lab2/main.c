#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFSIZE 512 /* 한 번에 읽을 문자열의 크기 */
#define PERM 0644   /* 새로 만든 파일의 사용 권한 */

/**
 * 파일을 복사하여 사본을 생성한다.
 * @param name1 원본 파일 이름
 * @param name2 사본 파일 이름
 * @return 처리 결과를 숫자 형태로 반환 (자유롭게 정의하여 사용)
 */
int copy_file(const char *name1, const char *name2);
void create_holed_file(const char *str);
void fatal(const char *str, int errno);

int main()
{
  create_holed_file("file.hole");
  return copy_file("file.hole", "file2.hole");
}

int copy_file(const char *name1, const char *name2)
{
  // return -1 -> error
  // return 1 -> success

  int fd1, fd2, read_n;
  char *buff = malloc(BUFSIZE);
  mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;

  // open name1, name2
  if ((fd1 = open(name1, O_RDONLY)) < 0)
  {
    return -1;
  }
  if ((fd2 = open(name2, O_CREAT | O_WRONLY | O_TRUNC, mode)) < 0)
  {
    return -1;
  }

  do
  {
    if ((read_n = read(fd1, buff, BUFSIZE)) < 0)
    {
      return -1;
    }

    if ((write(fd2, buff, read_n)) < 0)
    {
      return -1;
    }
  } while (read_n == BUFSIZE);

  close(fd1);
  close(fd2);
  free(buff);
  return 1;
}

void create_holed_file(const char *str)
{
  char buf1[] = "abcde\nfghij", buf2[] = "ABCDEFGHIJ";
  mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;
  int fd;

  if ((fd = open(str, O_CREAT | O_WRONLY | O_TRUNC, PERM)) < 0)
    fatal("creat error", 1);

  if (write(fd, buf1, 10) != 10)
    fatal("buf1 write error", 1);

  if (lseek(fd, 40, SEEK_SET) == -1)
    fatal("lseek error", 1);

  if (write(fd, buf2, 10) != 10)
    fatal("buf2 write error", 1);
}

void fatal(const char *str, int errcode)
{
  perror(str);
  exit(errcode);
}
