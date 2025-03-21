#ifndef NARGS
#define NARGS 4
#endif
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	// Your code here
	int fd[2];
	char buffer[100];
	int bytes_leidos = read(0, &buffer, sizeof(buffer));
	buffer[bytes_leidos] = "\0";
	printf("%s", buffer);

	return 0;
}
