#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* import pppd vars */
#define MAXNAMELEN	256
extern char user[MAXNAMELEN];
void dbglog __P((char *, ...));	/* log a debug message */
void info __P((char *, ...));	/* log an informational message */
void notice __P((char *, ...));	/* log a notice-level message */
void warn __P((char *, ...));	/* log a warning message */
void error __P((char *, ...));	/* log an error message */
void fatal __P((char *, ...));	/* log an error message and die(1) */
extern int (*pap_check_hook) __P((void));
extern int (*chap_check_hook) __P((void));
extern int (*pap_auth_hook) __P((char *user, char *passwd, char **msgp,
				 struct wordlist **paddrs,
				 struct wordlist **popts));

#define RANGA_LIBEXEC "/lib/ranga/libexec/scdial"

char pppd_version[] = "2.4.7";

static int check()
{
	return 1;
}

static int _pap_auth_hook(char *user, char *passwd, char **msgp,
		     struct wordlist **paddrs,
		     struct wordlist **popts)
{
	const char *username = user;
	const char *interface = strchr(user, '@');

	if (!interface) {
		info("scplugin: no interface name found, failback to netkeeper");
		interface = "netkeeper";
	} else {
		*((char *)interface) = '\0';
		interface++;
	}

	info("scplugin: username: %s, interface: %s", username, interface);

	pid_t pid;
	int status;

	sigset_t set, oset;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, &oset);

	if ((pid = fork()) == 0) {
		execl(RANGA_LIBEXEC, RANGA_LIBEXEC, username, interface, NULL);
		info("scplugin: run libexec failed");
		_exit(0);
	}

	if (pid > 0) {
		while (waitpid(pid, &status, 0) != pid) {
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
	}

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_UNBLOCK, &set, &oset);

	info("scplugin: pap hook end");
	return 0;
}

void plugin_init(void)
{
	info("scplugin: ==================");
	info("copyright NSWA Maintainers");

	pap_check_hook = check;
	chap_check_hook = check;

	//chap_verify_hook = _chap_verify_hook;
	pap_auth_hook = _pap_auth_hook;
}
