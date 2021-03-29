#include <unistd.h>
#include <stdio.h>

int main(){
	if(access("test", F_OK) < 0){
		perror("access: ");
		return -1;
	}

	if(link("test", "testl") < 0){
		perror("link: ");
		return -1;
	}
	printf("success\n");
	return 0;
}
