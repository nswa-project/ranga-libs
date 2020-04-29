/* 
 * copyright 2019 NSWA Maintainers
 * copyright 2019 The-Seth-Project
 * License: MIT
 * This is free software with ABSOLUTELY NO WARRANTY.
 */

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

#define LIBSETH_APPLICATION_PORTABLE_FOR_POSIX
#include "seth.h"
#include "portable.h"

LIBSETH_APPLICATION("comp.nswa.seth_ranga")

static ssize_t readn(int fd, void *ptr, size_t n)
{
	size_t nleft;
	ssize_t nread;

	nleft = n;
	while (nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (nleft == n)
				return (-1);	/* error, return -1 */
			else
				break;	/* error, return amount read so far */
		} else if (nread == 0) {
			break;	/* EOF */
		}
		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);	/* return >= 0 */
}

static const char *get_isp_name(uint16_t ispcode)
{
	switch (ispcode) {
	case SETH_NK_ISP_UNSPEC:
		return "unspecified";
	case SETH_NK_ISP_CMCC_SD:
		return "CMCC Shandong";
	}
	return "Unknown";
}

static const char *get_netkeeper_version(uint16_t nkversion)
{
	switch (nkversion) {
	case SETH_NK_VERSION_INV:
		return "invalid";
	case SETH_NK_VERSION_2_5:
		return "2.5.0059";
	case SETH_NK_VERSION_4_9:
		return "4.9.19.699";
	case SETH_NK_VERSION_4_11:
		return "4.11.4.731";
	case SETH_NK_VERSION_5:
		return "5.x";
	}
	return "Unknown";
}

int seth_application_main(int argc, char *argv[])
{
	int exitval = 0;
	int fd, flags;
	struct seth_v2_hdr hdr;

	if (argc < 1) {
		printf("usage: %s <file>\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY | O_NOATIME)) < 0) {;
		exitval = 1;
		fprintf(stderr, "open error\n");
		goto err_open;
	}

	if (readn(fd, &hdr, sizeof(struct seth_v2_hdr)) !=
	    sizeof(struct seth_v2_hdr)) {
		exitval = 2;
		fprintf(stderr, "read error\n");
		goto err_read_hdr;
	}

	if (seth_hdr_valid(&hdr) < 0) {
		exitval = 3;
		fprintf(stderr, "Seth data invalid\n");
		goto err_hdr_invalid;
	}

	off_t fsize;
	if ((fsize = lseek(fd, 0, SEEK_END)) == -1) {
		exitval = 4;
		perror("lseek error");
		goto err_lseek;
	}

	size_t hdrlen = (size_t) seth_hdr_get_offset(&hdr);
	size_t life = seth_hdr_get_life(&hdr);
	size_t length = seth_hdr_get_length(&hdr);

	printf("%zd:%zu",
		(ssize_t) seth_hdr_get_timestamp(&hdr),
		(size_t) ((fsize - hdrlen) / length * life));

	flags = seth_hdr_get_flags(&hdr);

err_lseek:
err_hdr_invalid:
err_read_hdr:
	close(fd);
err_open:
	exit(exitval);
}
