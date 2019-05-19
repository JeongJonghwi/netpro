#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char*);

int main(int argc, char *argv[]) {
	int sock, str_len;
	struct sockaddr_in serv_adr;
	socklen_t clnt_adr_sz;
	char buf[BUF_SIZE] = {0, };
	FILE *writefp, *readfp;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) error_handling("connect() error");
	else puts("connected......");

	readfp = fdopen(sock, "r");
	writefp = fdopen(dup(sock), "w");

	while (1) {
		fputs("input message(Q to quit): ", stdout);
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
			shutdown(fileno(writefp), SHUT_WR);
			fclose(writefp);
			fputs("implement: shutdown(fileno(writefp), SHUT_WR);\n", stdout);
			break;
		}
		write(fileno(writefp), buf, strlen(buf));
		fflush(writefp);
		str_len = read(fileno(readfp), buf, BUF_SIZE -1);
		buf[str_len] = 0;
		printf("Message from server: %s", buf);	
	}
	str_len = read(fileno(readfp), buf, BUF_SIZE - 1);
	buf[str_len] = 0;
	printf("Last message from server: %s\n", buf);
	fclose(readfp);
	return 0;
}

void error_handling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
