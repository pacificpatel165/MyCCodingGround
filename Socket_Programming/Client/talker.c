#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>

#define MYPORT 4950    // the port users will be connecting to

char * my_print(const char *fmt, ...);

int main()
{
	int i=90;
  	float f=23.45;
  	my_print("prashant---->%s rohit---->%d roj--->%f  aelix---->%s\n", "patel",i,f,"communication systems");
}

char *my_print(const char *fmt, ...) 
{
	/* Guess we need no more than 100 bytes. */
  	int n, size = 20;
  	int sockfd;
  	int numbytes;
  	//char *hostname="prashant";
  	char *msg;
  	va_list ap;
  	struct sockaddr_in their_addr; // connector's address information
  	struct hostent *he;

  	if ((msg = malloc (size)) == NULL)
  		return NULL;
  
  	while (1) 
  	{
    	/* Try to print in the allocated space. */
    	va_start(ap, fmt);
    	n = vsnprintf (msg, size, fmt, ap);
    	va_end(ap);
    	/* If that worked, return the string. */
    	if (n > -1 && n < size)
    	{
			/*
 			if ((he=gethostbyname(hostname)) == NULL) 
  			{  // get the host info
        		perror("gethostbyname");
        		exit(1);
    		}
			*/

    		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
			{
        		perror("socket");
        		exit(1);
    		}

    		their_addr.sin_family = AF_INET;     // host byte order
    		their_addr.sin_port = htons(MYPORT); // short, network byte order
    		//their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    		their_addr.sin_addr.s_addr = inet_addr("192.168.0.139");
    		//inet_pton(AF_INET,"192.168.0.44",&their_addr.sin_addr);
    		memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct
    
    		if ((numbytes=sendto(sockfd, msg, strlen(msg), 0,
         			(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) 
			{
        		perror("sendto");
        		exit(1);
    		}

    		printf("sent %d bytes to %s\n", numbytes,inet_ntoa(their_addr.sin_addr));
   
    		close(sockfd);

    		return 0;

    	}
    	/* Else try again with more space. */
    	if (n > -1)    /* glibc 2.1 */
    		size = n+1; /* precisely what is needed */
    	else           /* glibc 2.0 */
    		size *= 2;  /* twice the old size */
    	if ((msg = realloc (msg, size)) == NULL)
		{
    		return NULL;
   		}
  	} 
}
