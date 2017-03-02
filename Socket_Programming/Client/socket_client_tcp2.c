/* This is a client program which will connect to the server across 
 * network using TCP protocol */

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

	/* Opens a TCP socket(an Internet strem socket) */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Fill in the structure "address" with the address of the server
 	*  that we want to connect with .*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("10.48.10.2");
	address.sin_port = 9005;
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
	
