#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <grp.h>

int main(int argc, char* argv[]) {
	if (argc < 3)
		printf("Usage: %s <uid> <program> <...>", argv[0]), exit(1);

	if (setuid(0) || setgid(0)) {
		perror("setuid");
		exit(1);
	}

	setgroups(0, NULL);

	uid_t uid = atol(argv[1]);

	if (setgid(uid) || setuid(uid)) {
		perror("setuid");
		exit(1);
	}

	char* args[argc - 1];
	for (int i = 0; i < argc - 2; i++) {
		args[i] = argv[i + 2];
	}
	args[argc - 2] = NULL;

	execv(argv[2], args);
	exit(1);
}
