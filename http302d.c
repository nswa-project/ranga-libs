#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<pthread.h>

#define MAXLINE 4096

char msg1[] = "HTTP/1.1 302 Found\r\nLocation: http://";
char msg2[] = "\r\nConnection: close\r\nCache-Control: no-cache, no-store, must-revalidate\r\n\r\n";
const char *ip = NULL;
size_t len = 0;

int listenfd;

void *connection_handler(void *socket_desc)
{
	char buf[1024];
	int connfd = *(int *) socket_desc;
	while (recv(connfd, buf, 1024, MSG_DONTWAIT) > 0) {
	}
	write(connfd, msg1, sizeof(msg1) - 1);
	write(connfd, ip, len);
	write(connfd, msg2, sizeof(msg2) - 1);

	while (recv(connfd, buf, 1024, MSG_DONTWAIT) > 0) {
	}
	close(connfd);
	return 0;
}

int main(int argc, char **argv)
{
	ip = argv[1];
	len = strlen(ip);

	struct sockaddr_in servaddr;

	signal(SIGPIPE, SIG_IGN);

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		exit(1);
	}

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int));

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(81);

	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))
	    == -1) {
		exit(2);
	}

	if (listen(listenfd, 20) == -1) {
		exit(3);
	}

	int connfd;
	pthread_t thread_id;
	while (1) {
		if ((connfd = accept(listenfd, NULL, NULL)) == -1) {
			continue;
		}

		if (pthread_create
		    (&thread_id, NULL, connection_handler,
		     (void *) &connfd) < 0) {
			close(connfd);
		}
	}

	close(listenfd);
}
