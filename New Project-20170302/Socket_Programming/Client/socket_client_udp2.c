/* This is a client program which will send datagram to the server 
 * across network using UDP protocol. */

#include "sock.h"

int main()
{
	int cli_sockfd;
	char buff[64];
	int ser_len, cli_len, n;
	struct sockaddr_in ser_addr, cli_addr;
	cli_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	cli_addr.sin_port = 9016;
	cli_len = sizeof(cli_addr);

	/* Binding an address to the client socket for connectionless 
 	* protocol. */

	bind(cli_sockfd, (struct sockaddr *)&cli_addr, cli_len);
	strcpy(buff, "It's my life\n");
	n = strlen(buff);
	buff[n] = '\0';
	n++;

	/* Fill in the structure "ser_addr" with the address of the server
 	* tha we want to send datagram to. */
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr("10.48.10.2");
	ser_addr.sin_port = 9015;
	ser_len = sizeof(ser_addr);

	/* Sending datagram to the server using sendto system call */
	if(sendto(cli_sockfd, buff, n, 0, (struct sockaddr *)&ser_addr,ser_len) != n)
		perror("Error in sendto\n");
	printf("waiting for server...\n");
	/* Receiving data from the sever after processing it */
	if(recvfrom(cli_sockfd, buff, 64, 0, (struct sockaddr *)&ser_addr,(socklen_t *)&ser_len) < 0)
		perror("Error in recvfrom\n");
	printf("data received\n");
	printf("String from client %s\n", buff);
}
