/************tcpserver.c************************/
/* header files needed to use the sockets API */
/* File contain Macro, Data Type and Structure */
/***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

/* BufferLength is 100 bytes */
#define BufferLength 100

/* Server port number */
#define SERVPORT 3111 

int main()
{
	/* Variable and structure definitions. */
	int sd, sd2, rc, length = sizeof(int);
	int totalcnt = 0, on = 1;
	char temp;
	char buffer[BufferLength];
	struct sockaddr_in serveraddr;
	struct sockaddr_in their_addr;

	fd_set read_fd;
	struct timeval timeout;
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;

	/* The socket() function returns a socket descriptor */
	/* representing an endpoint. The statement also */
	/* identifies that the INET (Internet Protocol) */
	/* address family with the TCP transport (SOCK_STREAM) */
	/* will be used for this socket. */
	/************************************************/

	/* Get a socket descriptor */
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Server-socket() error");
		/* Just exit */
		exit (-1);
	}
	else
		printf("Server-socket() is OK\n");

	/* The setsockopt() function is used to allow */
	/* the local address to be reused when the server */
	/* is restarted before the required wait time */
	/* expires. */
	/***********************************************/

	/* Allow socket descriptor to be reusable */
	if((rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0)
	{
		perror("Server-setsockopt() error");
		close(sd);
		exit (-1);
	}
	else
		printf("Server-setsockopt() is OK\n");

	/* bind to an address */
	memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
	printf("Using %s, listening at %d\n", inet_ntoa(serveraddr.sin_addr), SERVPORT);
 
	/* After the socket descriptor is created, a bind() */
	/* function gets a unique name for the socket. */
	/* In this example, the user sets the */
	/* s_addr to zero, which allows the system to */
	/* connect to any client that used port 3005. */

	if((rc = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
	{
		perror("Server-bind() error");
		/* Close the socket descriptor */
		close(sd);
		/* and just exit */
		exit(-1);
	}
	else
    	printf("Server-bind() is OK\n");

	/* The listen() function allows the server to accept */
	/* incoming client connections. In this example, */
	/* the backlog is set to 10. This means that the */
	/* system can queue up to 10 connection requests before */
	/* the system starts rejecting incoming requests.*/
	/*************************************************/

	/* Up to 10 clients can be queued */
	if((rc = listen(sd, 10)) < 0)
	{
    	perror("Server-listen() error");
	    close(sd);
	    exit (-1);
	}
	else
    	printf("Server-Ready for client connection...\n");

	/* The server will accept a connection request */
	/* with this accept() function, provided the */
	/* connection request does the following: */
	/* - Is part of the same address family */
	/* - Uses streams sockets (TCP) */
	/* - Attempts to connect to the specified port */
	/***********************************************/

	/* accept() the incoming connection request. */
	int sin_size = sizeof(struct sockaddr_in);
	if((sd2 = accept(sd, (struct sockaddr *)&their_addr, &sin_size)) < 0)
	{
    	perror("Server-accept() error");
	    close(sd);
	    exit (-1);
	}
	else
	    printf("Server-accept() is OK\n");

	/*client IP*/
	printf("Server-new socket, sd2 is OK...\n");
	printf("Got connection from the f***ing client: %s\n", inet_ntoa(their_addr.sin_addr));

	/* The select() function allows the process to */
	/* wait for an event to occur and to wake up */
	/* the process when the event occurs. In this */
	/* example, the system notifies the process */
	/* only when data is available to read. */
	/***********************************************/

	/* Wait for up to 15 seconds on */
	/* select() for data to be read. */
	FD_ZERO(&read_fd);
	FD_SET(sd2, &read_fd);

	rc = select(sd2+1, &read_fd, NULL, NULL, &timeout);
	if((rc == 1) && (FD_ISSET(sd2, &read_fd)))
	{
		/* Read data from the client. */
		totalcnt = 0;

		while(totalcnt < BufferLength)
		{
			/* When select() indicates that there is data */
			/* available, use the read() function to read */
			/* 100 bytes of the string that the */
			/* client sent. */
			/***********************************************/

			/* read() from client */
			rc = read(sd2, &buffer[totalcnt], (BufferLength - totalcnt));
			if(rc < 0)
			{
			    perror("Server-read() error");
			    close(sd);
			    close(sd2);
    			exit (-1);
			}
			else if (rc == 0)
			{
			    printf("Client program has issued a close()\n");
			    close(sd);
			    close(sd2);
			    exit(-1);
			}
			else
			{
    			totalcnt += rc;
    			printf("Server-read() is OK\n");
			}
		}
	}
	else if (rc < 0)
	{
    	perror("Server-select() error");
    	close(sd);
    	close(sd2);
    	exit(-1);
	}
	/* rc == 0 */
	else 
	{
    	printf("Server-select() timed out.\n");
    	close(sd);
    	close(sd2);
    	exit(-1);
	}

	/* Shows the data */
	printf("Received data from the f***ing client: %s\n", buffer);

	/* Echo some bytes of string, back */
	/* to the client by using the write() */
	/* function. */
	/************************************/
	/* write() some bytes of string, */
	/* back to the client. */
	printf("Server-Echoing back to client...\n");
	rc = write(sd2, buffer, totalcnt);
	if(rc != totalcnt)
	{
		perror("Server-write() error");
		/* Get the error number. */
		rc = getsockopt(sd2, SOL_SOCKET, SO_ERROR, &temp, &length);
		if(rc == 0)
		{
		    /* Print out the asynchronously */
		    /* received error. */
		    errno = temp;
		    perror("SO_ERROR was: ");
		}
		else
		    printf("Server-write() is OK\n");

		close(sd);
		close(sd2);
		exit(-1);
	}

	/* When the data has been sent, close() */
	/* the socket descriptor that was returned */
	/* from the accept() verb and close() the */
	/* original socket descriptor. */
	/*****************************************/
	/* Close the connection to the client and */
	/* close the server listening socket. */
	/******************************************/
	close(sd2);
	close(sd);
	exit(0);
	return 0;
}

#if 0
Compile and link the program.  Make sure there is no error.

[bodo@bakawali testsocket]$ gcc -g tcpserver.c -o tcpserver

Run the program.  In this example we let the program run in the background.

[bodo@bakawali testsocket]$ ./tcpserver

Server-socket() is OK

Server-setsockopt() is OK

Using 0.0.0.0, listening at 3111

Server-bind() is OK

Server-Ready for client connection...

 

[1]+  Stopped                 ./tcpserver

[bodo@bakawali testsocket]$ bg

[1]+ ./tcpserver &

[bodo@bakawali testsocket]$

Do some verification.

[bodo@bakawali testsocket]$ ps aux | grep tcpserver

bodo      7914  0.0  0.2  3172  324 pts/3    S    11:59   0:00 ./tcpserver

bodo      7921  0.0  0.5  5540  648 pts/3    S+   12:01   0:00 grep tcpserver

[bodo@bakawali testsocket]$ netstat -a | grep 3111

tcp        0      0 *:3111              *:*                   LISTEN

When the next program example (the TCP client) is run, the following messages should be expected at the server console.

[bodo@bakawali testsocket]$ Server-accept() is OK

Server-new socket, sd2 is OK...

Got connection from the f***ing client: 203.106.93.94

Server-read() is OK

Received data from the f***ing client: This is a test string from client lol!!!

Server-Echoing back to client...

 

[1]+  Done                    ./tcpserver

[bodo@bakawali testsocket]$

If the server program and then the client are run, the following messages should be expected at the server console.

[bodo@bakawali testsocket]$ ./tcpserver

Server-socket() is OK

Server-setsockopt() is OK

Using 0.0.0.0, listening at 3111

Server-bind() is OK

Server-Ready for client connection...

Server-accept() is OK

Server-new socket, sd2 is OK...

Got connection from the f***ing client: 203.106.93.94

Server-read() is OK

Received data from the f***ing client: This is a test string from client lol!!!

Server-Echoing back to client...

[bodo@bakawali testsocket]$

Just telneting the server.

[bodo@bakawali testsocket]$ telnet   203.106.93.94   3111

Trying 203.106.93.94...

Connected to bakawali.jmti.gov.my (203.106.93.94).

Escape character is '^]'.

^]

telnet> help

Commands may be abbreviated.  Commands are:

 

close           close current connection
logout          forcibly logout remote user and close the connection
display         display operating parameters
mode            try to enter line or character mode ('mode ?' for more)
open            connect to a site
quit            exit telnet
send            transmit special characters ('send ?' for more)
set             set operating parameters ('set ?' for more)
unset           unset operating parameters ('unset ?' for more)
status          print status information
toggle          toggle operating parameters ('toggle ?' for more)
slc             change state of special charaters ('slc ?' for more)
auth            turn on (off) authentication ('auth ?' for more)
encrypt         turn on (off) encryption ('encrypt ?' for more)
forward         turn on (off) credential forwarding ('forward ?' for more)
z               suspend telnet
!               invoke a subshell
environ         change environment variables ('environ ?' for more)
?               print help information

telnet>quit

Well, it looks that we have had a telnet session with the server.

#endif
