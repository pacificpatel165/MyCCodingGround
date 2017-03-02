/* This is a client program which will connect to the server process in
 * the same machine using loopback network. */

#include "sock.h"

int main()
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	struct val value;
	printf("Enter two values\n");
	fflush(stdout);
	scanf("%d", &value.tmp);
	scanf("%d", &value.pin);

	/* Opens a stream socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Binds the socket with standard IP address neede for loopback network */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 9000;
	len = sizeof(address);

	/* Connecting to the server */
	
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1)
	{
		perror("client2");
		exit(1);
	}
	write(sockfd, &value, sizeof(struct val));
	read(sockfd, &value, sizeof(struct val));
	printf("char from server = %d, %d\n", value.tmp, value.pin);
	close(sockfd);
	exit(0);
}
	
