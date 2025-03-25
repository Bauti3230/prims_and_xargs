#ifndef NARGS
#define NARGS 4
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>

void
free_buffer(char **buffer, int amm)
{
	for (int i = 1; i <= amm; i++) {
		if (buffer[i] != NULL) {
			free(buffer[i]);
			buffer[i] = NULL;
		}
	}
}

void
xargs_diy(char *command)
{
	char *buffer[NARGS + 2];
	size_t characters;
	int count;

	while (1) {
		count = 0;
		buffer[0] = command;

		for (int i = 0; i < NARGS; i++) {
			size_t buffer_length = 0;
			buffer[i + 1] = NULL;

			characters =
			        getline(&buffer[i + 1], &buffer_length, stdin);

			if ((int) characters == -1) {
				break;
			}

			if (characters == 1 && buffer[i + 1][0] == '\n') {
				free(buffer[i + 1]);
				buffer[i + 1] = NULL;
				break;
			}

			if (characters > 0 &&
			    buffer[i + 1][characters - 1] == '\n') {
				buffer[i + 1][characters - 1] = '\0';
			}

			count++;
		}

		if (count == 0) {
			break;
		}

		buffer[count + 1] = NULL;

		pid_t pid = fork();

		if (pid == 0) {
			execvp(command, buffer);
			perror("execvp");
			exit(1);
		} else if (pid < 0) {
			perror("fork");
			exit(1);
		} else {
			wait(NULL);
			free_buffer(buffer, count);
		}
	}
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Uso : %s <comando>", argv[0]);
		return 1;
	}

	xargs_diy(argv[1]);

	return 0;
}