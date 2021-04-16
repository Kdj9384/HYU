#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>  // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h> // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h> // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <strings.h>

int main(void)
{
  char buff[256];
  printf("%d", sizeof(buff));
  char *header, header2;

  header = "HTTP/1.1 200 OK\r\n"
           "Date: Fri, 01 Feb 2002 01:34:56 GMT\n"
           "Server: Apache/1.3.19 (Unix) PHP/4.0.6\n"
           "X-Powered-By: PHP/4.0.6\n"
           "Connection: close\n"
           "Content-Type: text/html; charset=utf-8\n\n";

  header2 = "HTTP/1.1 200 OK\r\nDate: Fri, 01 Feb 2002 01:34:56 GMT\nServer: Apache/1.3.19 (Unix) PHP/4.0.6\n";
}