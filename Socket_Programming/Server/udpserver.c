/*******************udpserver.c*****************/

/* Header files needed to use the sockets API. */

/* File contain Macro, Data Type and Structure */

/* definitions along with Function prototypes. */

/* header files */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

/* Server's port number, listen at 3333 */

#define SERVPORT 3333

 

/* Run the server without argument */

int main(int argc, char *argv[])

{

/* Variable and structure definitions. */

int sd, rc;

struct sockaddr_in serveraddr, clientaddr;

int clientaddrlen = sizeof(clientaddr);

int serveraddrlen = sizeof(serveraddr);

char buffer[100];

char *bufptr = buffer;

int buflen = sizeof(buffer);

 

/* The socket() function returns a socket */

/* descriptor representing an endpoint. */

/* The statement also identifies that the */

/* INET (Internet Protocol) address family */

/* with the UDP transport (SOCK_DGRAM) will */

/* be used for this socket. */

/******************************************/

/* get a socket descriptor */

if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)

{

perror("UDP server - socket() error");

exit(-1);

}

else

printf("UDP server - socket() is OK\n");

 

printf("UDP server - try to bind...\n");

 

/* After the socket descriptor is received, */

/* a bind() is done to assign a unique name */

/* to the socket. In this example, the user */

/* set the s_addr to zero. This allows the */

/* system to connect to any client that uses */

/* port 3333. */

/********************************************/

/* bind to address */

memset(&serveraddr, 0x00, serveraddrlen);

serveraddr.sin_family = AF_INET;

serveraddr.sin_port = htons(SERVPORT);

serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

if((rc = bind(sd, (struct sockaddr *)&serveraddr, serveraddrlen)) < 0)

{

perror("UDP server - bind() error");

close(sd);

/* If something wrong with socket(), just exit lol */

exit(-1);

}

else

printf("UDP server - bind() is OK\n");

 

printf("Using IP %s and port %d\n", inet_ntoa(serveraddr.sin_addr), SERVPORT);

printf("UDP server - Listening...\n");

 

/* Use the recvfrom() function to receive the */

/* data. The recvfrom() function waits */

/* indefinitely for data to arrive. */

/************************************************/

/* This example does not use flags that control */

/* the reception of the data. */

/************************************************/

/* Wait on client requests. */

rc = recvfrom(sd, bufptr, buflen, 0, (struct sockaddr *)&clientaddr, &clientaddrlen);

if(rc < 0)

{

perror("UDP Server - recvfrom() error");

close(sd);

exit(-1);

}

else

printf("UDP Server - recvfrom() is OK...\n");

 

printf("UDP Server received the following:\n \"%s\" message\n", bufptr);

printf("from port %d and address %s.\n", ntohs(clientaddr.sin_port),

inet_ntoa(clientaddr.sin_addr));

 

/* Send a reply by using the sendto() function. */

/* In this example, the system echoes the received */

/* data back to the client. */

/************************************************/

/* This example does not use flags that control */

/* the transmission of the data */

/************************************************/

/* Send a reply, just echo the request */

printf("UDP Server replying to the stupid UDP client...\n");

rc = sendto(sd, bufptr, buflen, 0, (struct sockaddr *)&clientaddr, clientaddrlen);

if(rc < 0)

{

perror("UDP server - sendto() error");

close(sd);

exit(-1);

}

else

printf("UDP Server - sendto() is OK...\n");

 

/* When the data has been sent, close() the */

/* socket descriptor. */

/********************************************/

/* close() the socket descriptor. */

close(sd);

exit(0);

}

#if 0

Compile and link the udp server program.

[bodo@bakawali testsocket]$ gcc -g udpserver.c -o udpserver

Run the program and let it run in the background.

[bodo@bakawali testsocket]$ ./udpserver

UDP server - socket() is OK

UDP server - try to bind...

UDP server - bind() is OK

Using IP 0.0.0.0 and port 3333

UDP server - Listening...

 

[1]+  Stopped                 ./udpserver

[bodo@bakawali testsocket]$ bg

[1]+ ./udpserver &

[bodo@bakawali testsocket]$

Verify the program running.

[bodo@bakawali testsocket]$ ps aux | grep udpserver

bodo      7963  0.0  0.2  2240  324 pts/2    S    12:22   0:00 ./udpserver

bodo      7965  0.0  0.5  4324  648 pts/2    S+   12:24   0:00 grep udpserver

Verify that the udp server is listening at port 3333 waiting for the client connection.

[bodo@bakawali testsocket]$ netstat -a | grep 3333

udp        0      0 *:3333                      *:*

[bodo@bakawali testsocket]$

Without the client program (next example) you can try telneting the server using port 3333 for testing.  For this program example the following telnet session cannot be established for UDP/connectionless.

[bodo@bakawali testsocket]$ telnet 203.106.93.94 3333

Trying 203.106.93.94...

telnet: connect to address 203.106.93.94: Connection refused

telnet: Unable to connect to remote host: Connection refused



#endif
