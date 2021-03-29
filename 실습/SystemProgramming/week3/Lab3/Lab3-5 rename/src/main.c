#include <stdio.h>

int main(){
	if(rename("test", "test2") < 0){
		perror("rename: ");
	} else {
		printf("success\n");
	}
	return 0;
}
