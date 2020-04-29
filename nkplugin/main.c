/* 
 * copyright 2019 glider0
 * License: MIT
 * This is free software with ABSOLUTELY NO WARRANTY.
 */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LIBSETH_APPLICATION_PORTABLE_FOR_POSIX
#include "seth.h"
#include "nkget.h"

LIBSETH_APPLICATION("comp.nswa.rangaplugin")

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

char pppd_version[] = "2.4.7";

typedef unsigned char byte;

static int check()
{
	return 1;
}

static int seth_override_by_sth_file(const char *fname)
{
	char *papreal;
	int err;

	if ((err = seth_nkpin_get_from_file(fname, NULL, time(NULL), &papreal)) < 0) {
		info("seth_nkpin_get_from_file error, errcode: %d\n", err);
		return -1;
	}

	strncpy(user, papreal, MAXNAMELEN);
	free(papreal);

	return 0;
}

static int seth_try_sth_file(const char *format)
{
	char filename[PATH_MAX];
	snprintf(filename, PATH_MAX, format, user);
	info("seth: trying data file '%s'", filename);

	if (access(filename, F_OK) == 0) {
		return seth_override_by_sth_file(filename);
	}

	return -1;
}

static int seth_override()
{
	info("nkplugin: loadmod: ss-seth");
	if (seth_try_sth_file("/etc/seth/%s.sth3") == 0)
		return 0;

	return -1;
}

void plugin_init()
{
	info("netkeeper plugin: ===============");
	info("nkplugin: v4.7.13 (enabled features: --nkoverride --nk2-cmcc-sd --ss-seth)");

	info("nkplugin: loadmod: nkoverride");
	FILE *f = fopen("/tmp/nk-override", "r");
	if (f) {
		info("nkoverride: found server-catched username");
		user[0] = 0;
		fgets(user, MAXNAMELEN, f);
		fclose(f);
		unlink("/tmp/nk-override");
	} else if (seth_override() != 0) {
		info("nkplugin: loadmod: nk2-cmcc-sd");
		info("nkplugin: nk2-cmcc-sd: stub");
	}

	info("nkplugin: override username: %s", user);
	pap_check_hook = check;
	chap_check_hook = check;
	info("netkeeper plugin: end ===========");
}
