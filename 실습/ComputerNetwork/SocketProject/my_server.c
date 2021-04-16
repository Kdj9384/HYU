/**
 * @file my_server.c
 * @author 강 동진 (2017012642)
 * @brief 
 * 
 * 1. my_client로 부터 connection 요청이 오면 response를 보낸다.
 * 2. my_client로 부터 온 HTTPrequest를 파싱하여 HTTPresponse를 보내준다.
 * 3. close()
 *  
 * @version 0.1
 * @date 2021-04-09
 * 
 * TODO: ## Todo
 * - [o] response header 수정하기
 * - [o] GIF
 * - [o] JPEG
 * - [o] MP3
 * - [o] PDF
 * @copyright Copyright (c) 2021
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define BACKLOG_SIZE 5
#define BUFF_SIZE 1024

void sigHandler(int sig)
{
  exit(1);
}

char *check_day_name(int day_name_int)
{
  switch (day_name_int)
  {
  case 0:
    return "Mon";
    break;

  case 1:
    return "Tue";
    break;
  case 2:
    return "Wed";
    break;
  case 3:
    return "Thu";
    break;
  case 4:
    return "Fri";
    break;
  case 5:
    return "Sat";
    break;
  case 6:
    return "Sun";
    break;
  }

  return "Can not reach";
}

char *check_month(int month_int)
{
  switch (month_int)
  {
  case 0:
    return "Jan";
    break;

  case 1:
    return "Feb";
    break;
  case 2:
    return "Mar";
    break;
  case 3:
    return "Apr";
    break;
  case 4:
    return "May";
    break;
  case 5:
    return "Jun";
    break;
  case 6:
    return "Jul";
    break;
  case 7:
    return "Aug";
    break;
  case 8:
    return "Sep";
    break;
  case 9:
    return "Oct";
    break;
  case 10:
    return "Nov";
    break;
  case 11:
    return "Dec";
    break;
  }

  return "can not reach";
}

char *check_content_type(char *ptr)
{
  char *dot = strrchr(ptr, '.');
  if (!dot)
  {
    perror("no such file");
  }

  dot = dot + 1;

  // html -> text/html
  if (!strcmp(dot, "html"))
  {
    return "text/html";
  }

  // jpeg -> image/jpeg
  else if (!strcmp(dot, "jpeg"))
  {
    return "image/jpeg";
  }

  // pdf -> application/pdf
  else if (!strcmp(dot, "pdf"))
  {
    return "application/pdf";
  }

  // gif -> image/gif
  else if (!strcmp(dot, "gif"))
  {
    return "image/gif";
  }

  // mp3 -> audio/mpeg
  else if (!strcmp(dot, "mp3"))
  {
    return "audio/mpeg";
  }

  // 기타의 경우, 그냥 html로 보냄.
  else
  {
    return "text/html";
  }
}

void make_header(char header[], char *ptr)
{
  // 1. date 넣기
  // Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
  time_t rawtime;
  struct tm *tminfo;

  time(&rawtime);
  tminfo = localtime(&rawtime);

  int day_name_int = tminfo->tm_wday;
  int day = tminfo->tm_mday;
  int month_int = tminfo->tm_mon;
  int year = tminfo->tm_year + 1900;
  int hour = tminfo->tm_hour;
  int minute = tminfo->tm_min;
  int second = tminfo->tm_sec;

  char *day_name = check_day_name(day_name_int);
  char *month = check_month(month_int);

  // 2. content-type 파싱
  char *cnt_type = check_content_type(ptr);

  // 3. 파싱한 내용을 header에 입력.
  sprintf(header, "HTTP/1.1 200 OK\n"
                  "Date: %s, %d %s %d %d:%d:%d GMT\n"
                  "Server: LocalServer\n"
                  "Connection: close\n"
                  "Content-Type: %s; charset=utf-8n\n\n",
          day_name, day, month, year, hour, minute, second, cnt_type);
}

int main(int argc, char *argv[])
{
  int server_sock_fd, new_sock_fd;
  struct sockaddr_in serv_addr_in, client_addr_in;

  // argv가 정상적으로 입력되었는지 판단.
  if (argc != 2)
  {
    perror("too few parameter");
    exit(1);
  }

  // 종료를 위해 시그널 처리
  // signal(SIGINT, sigHandler);

  // create
  if ((server_sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("create socket error");
    exit(1);
  }

  // serv_addr_in 초기화
  bzero((char *)&serv_addr_in, sizeof(serv_addr_in));

  // serv_addr_in 구조체 구성
  serv_addr_in.sin_family = AF_INET;
  serv_addr_in.sin_port = htons(atoi(argv[1]));
  serv_addr_in.sin_addr.s_addr = INADDR_ANY;

  // bind
  if (bind(server_sock_fd, (struct sockaddr *)&serv_addr_in, sizeof(struct sockaddr_in)) < 0)
  {
    perror("bind socket error");
    exit(1);
  }

  // listen(fd, backlog queue size) : non-blocking, listen실행되면 바로 return 함.
  // backlog queue : 3 way hand shake에서 사용하는 SYN정보가 임시저장되는 버퍼.
  if (listen(server_sock_fd, BACKLOG_SIZE) < 0)
  {
    perror("listen error");
    exit(1);
  }
  socklen_t client_len = sizeof(client_addr_in);

  // accept : blocking, waits for connection before returning.
  while (1)
  {
    // accept를 통해 새로운 소켓 생성.
    if ((new_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_addr_in, &client_len)) < 0)
    {
      perror("accept error");
      exit(1);
    }

    // handshaking (3-way handshaking)
    printf("hand shaking complete!\n");

    char read_buff[BUFF_SIZE];
    char write_buff[BUFF_SIZE];
    char temp[BUFF_SIZE];
    int read_byte, write_byte;
    int cnt = 1;

    int fd;
    int n;
    char header[1024];
    int read_length;

    // read_buff 초기화
    bzero(read_buff, sizeof(read_buff));

    // request 읽기
    while (cnt)
    {
      if ((read_byte = read(new_sock_fd, read_buff, BUFF_SIZE)) < 0)
      {
        perror("read error");
        exit(1);
      }
      else if (read_byte < BUFF_SIZE)
      {
        printf("%s\n", read_buff);
        break;
      }

      // printf("%s\n", read_buff);
      cnt++;
    }

    // read_buff 복사
    strcpy(temp, read_buff);

    // read_buff 파싱하기
    char *ptr = strtok(temp, " "); // GET
    ptr = strtok(NULL, " ");       // /index.html or request 한 파일
    read_length = strlen(ptr);     // 파일 이름의 길이

    // 새로운 배열을 만든 다음, 파싱한 내용을 복사.
    char target_ptr[read_length + 1];
    target_ptr[0] = '.';
    strcat(target_ptr, ptr);

    // request한 파일 열기
    fd = open((const char *)target_ptr, O_RDONLY);
    if (fd < 0)
    {
      // 파일이 없을 경우 무시
      perror("no file");
      continue;
    }

    // write_buff 초기화
    bzero(write_buff, sizeof(write_buff));

    printf("before header\n");
    bzero(header, sizeof(header));
    // make header
    make_header(header, ptr);

    // write header
    write(new_sock_fd, header, strlen(header));

    // write data
    while ((n = read(fd, write_buff, BUFF_SIZE)) > 0)
    {
      write(new_sock_fd, write_buff, n);
      bzero(write_buff, sizeof(write_buff));
    }

    // close
    close(new_sock_fd);
  }
  // close
  close(server_sock_fd);
}
