#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <grp.h>

int main(int argc, char* argv[]) {
	if (argc < 3)
		printf("Usage: %s <wait time> <program> <...>", argv[0]), exit(1);

	long u = atol(argv[1]);

	setsid();

	sleep(u);

	char* args[argc - 1];
	for (int i = 0; i < argc - 2; i++) {
		args[i] = argv[i + 2];
	}
	args[argc - 2] = NULL;

	execv(argv[2], args);
	exit(1);
}
