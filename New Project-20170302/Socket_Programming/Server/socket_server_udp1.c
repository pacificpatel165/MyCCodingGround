/* This is a server program for connectionless using loopback */

#include "sock.h"

int main()
{
	int ser_sockfd;
	char buff[64];
	int ser_len, cli_len, n;
	struct sockaddr_in ser_addr, cli_addr;
	ser_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ser_addr.sin_port = 9010;
	ser_len = sizeof(ser_addr);

	bind(ser_sockfd, (struct sockaddr *)&ser_addr, ser_len);
	while(1)
	{
		cli_len = sizeof(cli_addr);
		printf("waiting for data...\n");
		recvfrom(ser_sockfd, buff, 64, 0, (struct sockaddr *)&cli_addr,(socklen_t *)&cli_len);
		printf("data received\n");
		printf("String from client %s\n", buff);
		strcpy(buff, "It's not my life");
		n = strlen(buff);
		buff[n] = '\0';
		n++;
		if(sendto(ser_sockfd, buff, n, 0, (struct sockaddr *)&cli_addr,cli_len) != n)
			perror("Error in sendto\n");
		else
			printf("data send is %s\n", buff);
	}
}
