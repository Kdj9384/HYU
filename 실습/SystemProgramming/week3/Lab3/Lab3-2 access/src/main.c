#include <stdio.h>
#include <unistd.h>

int main(){
	if(access("test", F_OK) != 0){
		perror("파일이 존재하지 않음");
	} else {
		printf("파일이 존재함\n");
		if(access("test", R_OK | W_OK) != 0){
			perror("읽기 쓰기 권한이 없음");
		} else {
			printf("읽기 쓰기 권한이 있음\n");
		}
		if(access("test", X_OK) != 0){
			perror("실행 권한이 없음");
		} else {
			printf("실행 권한이 있음\n");
		}
	}
	return 0;
}
