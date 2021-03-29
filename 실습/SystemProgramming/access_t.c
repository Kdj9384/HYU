#include <stdio.h>
#include <unistd.h>

/**
 * 
*/

int main()
{
    if (access("test_out", F_OK) != 0)
    {
        perror("파일이 존재하지 않음");
    } else {

        printf('파일 실행 권한 있ㄹ음');
    }
}