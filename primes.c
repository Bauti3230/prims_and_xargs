#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
prime_filter(int process[2])
{
	int p;
	if (read(process[0], &p, sizeof(p)) == 0) {
		close(process[0]);
		return 0;
	}

	printf("primo %i\n", p);

	int fd_der[2];
	pipe(fd_der);

	pid_t pid = fork();
	if (pid == 0) {
		close(process[0]);
		close(fd_der[1]);
		prime_filter(fd_der);
	} else if (p < 0) {
		printf("Ocurrio un error en un fork\n");
		return 1;
	} else {
		close(fd_der[0]);
		int num;
		while (read(process[0], &num, sizeof(num)) > 0) {
			if (num % p != 0) {
				write(fd_der[1], &num, sizeof(num));
			}
		}
		close(process[0]);
		close(fd_der[1]);
		wait(process);
		return 0;
	}
}


int
main(int argc, char *argv[])
{
	// Your code here
	if (argc != 2) {
		printf("Cantidad de argumentos invalida\n");
		return 1;
	}

	int n = atoi(argv[1]);
	if (n < 2) {
		printf("el numero ingresa (%i) deber ser > 2", n);
		return 1;
	}

	int fd[2];
	pipe(fd);

	for (int i = 2; i < n; i++) {
		write(fd[1], &i, sizeof(i));
	}
	close(fd[1]);

	return prime_filter(fd);
}