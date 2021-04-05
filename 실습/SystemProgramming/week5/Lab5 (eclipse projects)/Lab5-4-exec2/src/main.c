/* 빌드 시 echoall 제외하기
 * 프로젝트 이름 우클릭 > Properties > C/C++ General
 * > Paths and Symbols > Source Location
 * > /"프로젝트이름"/src 선택 후 Edit Filter
 * > Add > Browse > echoall.c 선택 후 OK
 * > OK를 반복 선택하여 창 닫기 > Apply and Close
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t pid;
	char path[3000];
	char *vector[] = {"echoall", "myarg1", "MY ARG2", (char *) 0};
	// embedding build command for echoall
	system("gcc ./src/echoall.c -o echoall");
	if((pid = fork()) < 0){
		perror("fork error");
		exit(1);
	} else if(pid == 0){ // 자식
		sprintf(path, "PATH=%s", getcwd(NULL, 0));
		char *env_init[] = {"USER=unknown", path, NULL};
		if(execve("echoall", vector, env_init) < 0){
			fprintf(stderr, "execve error\n");
			exit(1);
		}
	}// 부모
	if(waitpid(pid, NULL, 0) < 0){
		perror("waitpid error");
		exit(1);
	}
	if((pid = fork()) < 0){
		perror("fork2 error");
		exit(1);
	} else if(pid == 0) { // 자식 2
		if(execlp("echoall", "echoall", "onely one arg", (char *) 0) < 0){
			perror("execlp error");
			exit(1);
		}
	}
	// remove echoall in Debug directory
	system("rm ./Debug/echoall");
	return 0;
}
