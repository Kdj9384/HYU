#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

/**
 * @brief 
 * 1. 입력받은 파일명을 통해 접근 권한이 있는지 파악 ()
 * 2. 접근 권한이 있는 경우, chmod a+rX 를 수행함.
 * 3. 인자의 수만큼 반복함.
 * 
 * chmod 에서 소유자 처리를 해주기 때문에 따로 getuid() 나 getgid() 를 통해 소유자를 확인하지 않았다.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */

int main(int argc, char **argv)
{
    char *filename;
    struct stat file_info;
    struct passwd *file_pw;
    struct group *file_group;
    mode_t file_mode;

    if (argc < 2)
    {
        perror(" no file name : ");
    }

    for (int i = 1; i < argc; i++)
    {
        filename = argv[i];

        // 파일이 있는 지 확인.
        if (access(filename, F_OK) != 0)
        {
            // 파일이 없음 출력 후 다음 인자로 넘어가기
            perror("access : no file or directory exist");
            continue;
        }

        // 파일 정보 복사
        if (stat(filename, &file_info) < 0)
        {
            perror("stat: couldn't get state");
            continue;
        }

        file_mode = file_info.st_mode;

        // 해당 파일의 소유자 확인.
        file_pw = getpwuid(file_info.st_uid);
        file_group = getgrgid(file_info.st_gid);

        if (S_ISDIR(file_mode) == 1) // 디렉토리인 경우
        {
            if (chmod(filename, file_mode | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH) < 0)
            {
                perror("you don't have permission"); // 권한이 없다면 에러 발생
                continue;
            }
        }
        else // 디렉토리가 아닌경우
        {
            // 실행권한의 유무 파악
            if (access(filename, X_OK) != 0)
            {
                // 모든 읽기 권한만 부여.
                if (chmod(filename, file_mode | S_IRUSR | S_IRGRP | S_IROTH) < 0)
                {
                    perror("you don't have permission");
                    continue;
                }
            }
            else
            {
                // 모든 읽기, 실행 권한 부여.
                if (chmod(filename, file_mode | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH) < 0)
                {
                    perror("you don't have permission");
                    continue;
                }
            }
        }
    }
}
