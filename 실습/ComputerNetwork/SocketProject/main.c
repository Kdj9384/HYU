#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>

/**
 * @brief 
 * 요구사항
 * Part A : server dump request masseage to console. dump한 HTTPrequest 의 field
 * 를 분석
 * 
 * Part B : HTTPrequest를 parsing 하여, HTTPresponse를 통해 요청한 파일을 client에게
 * 직접적으로 전송.
 * 
 */

int main()
{
	char buff[256];
	printf("%d", sizeof(buff));
	char *header, *header2;

	char *ptr = "/index.html";
	char *dot = strrchr(ptr, '.');
	if (!dot)
	{
		perror("no such file");
	}

	dot = dot + 1;
	printf(" dot : %s", dot);
	// html -> text/html
	if (!strcmp(dot, "html"))
	{
		printf("html");
	}
}