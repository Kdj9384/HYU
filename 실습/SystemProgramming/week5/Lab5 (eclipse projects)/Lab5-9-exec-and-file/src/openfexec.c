#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char **argv) {
	int fd= atoi(argv[1]); /* I know it is 3 */
	long pos;
	pos = lseek(fd, 0l, SEEK_CUR);
	printf("\tPosin openfexec(): is %ld\n", pos);
	/* an arbitrary number */
	pos = lseek(fd, 50l, SEEK_CUR);
	printf("\tNewpos after lseek() in openfexec() is %ld\n", pos);
	/* return non-zero if not OK */
	return (pos < 0 ? !0 : 0);
}
