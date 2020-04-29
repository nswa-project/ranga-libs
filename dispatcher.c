#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <grp.h>

#define ARGS_MAX_LEN 12
#define ARG_MAX_SIZE 128

int main(int argc, char* argv[]) {
	if (argc < 2)
		printf("Usage: ... | %s <program>\n", argv[0]), exit(1);

	char *args[ARGS_MAX_LEN + 1];
	args[0] = argv[1];
	int i = 1;

	char *buffer = NULL;
	size_t n = 0;
	ssize_t len;

	while ((len = getline(&buffer, &n, stdin)) >= 0) {
		buffer[len - 1] = '\0';
		
		if ((args[i] = strdup(buffer)) == NULL)
			exit(4);

		i++;
		if (i > ARGS_MAX_LEN)
			exit(1);
	}

	args[i] = NULL;

	execv(argv[1], args);
	exit(4);
}
