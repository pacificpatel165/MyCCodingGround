/* This is a server program residing in the same machine as the client
 * process. This will create a socket and wait for client process to
 * connect */

#include "sock.h"

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address; 
	struct sockaddr_in client_address; 

	/* Opens a stream socket */
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Binds the socket with the standard IP address needed for loopback network */
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = 9000;
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	while(1)
	{
		struct val value;
		printf("server waiting \n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
		read(client_sockfd, &value, sizeof(struct val));
		printf("value from client %d, %d\n", value.tmp, value.pin);
		value.tmp = 0;
		value.pin = 0;
		write(client_sockfd, &value, sizeof(struct val));
		close(client_sockfd);
	}
}
