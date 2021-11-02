#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#define	LOG_NDEBUG	1

void sendCommandToRSD(char *cmd) {
	struct sockaddr_in server_addr;
	int client_len, sock_fd;

	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
	}
	memset(&server_addr, 0x0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_len = sizeof(server_addr);
	if (connect(sock_fd, (struct sockaddr*)&server_addr, client_len) == -1) {
		close(sock_fd);
	}
	write(sock_fd, cmd, strlen(cmd));
	close(sock_fd);
}
void setAppleDeviceSuspend(void) {
	sendCommandToRSD("suspend_mode");
}


