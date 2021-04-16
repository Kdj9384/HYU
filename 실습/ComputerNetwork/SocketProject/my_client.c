/**
 * @file my_client.c
 * @author 강 동진 (2017012642)
 * @brief 
 * 
 * 1. my_server에 connection 요청
 * 2. connection이 Establish 되면, 파일을 요청함.
 * 3. 파일을 수신함.
 * 4. close()
 * 
 * @version 0.1
 * @date 2021-04-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> // define structures like hostent
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[])
{
  int server_sock_fd;
  struct sockaddr_in serv_addr_in;

  // create
  if (server_sock_fd = socket(PF_INET, SOCK_STREAM, 0) < 0)
  {
    perror("create socket error");
  }

  // connect
  if (connect(server_sock_fd, (struct sockaddr *)&serv_addr_in, sizeof(struct sockaddr_in)) < 0)
  {
    perror("connect error");
  }
}