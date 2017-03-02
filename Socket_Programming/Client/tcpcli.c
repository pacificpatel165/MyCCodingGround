/* client.c */
 
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
 
int main(int argc,char *argv[])
{
	int create_socket,num;
  	int bufsize = 10024;
  	char *buffer = (char*)malloc(bufsize);
  	struct sockaddr_in address;
     
  	if ((create_socket = socket(AF_INET,SOCK_STREAM,0)) > 0)
  		printf("The Socket was created\n");

  	address.sin_family = AF_INET;
  	address.sin_port = htons(15000);
  	inet_pton(AF_INET,argv[1],&address.sin_addr);
   
  	if (connect(create_socket,(struct sockaddr *)&address,sizeof(address)) == 0)
  		printf("The connection was accepted with the server %s...\n",inet_ntoa(address.sin_addr));
   
  	do
  	{	
    	/*------receive data from server--------*/ 
    	strcpy(buffer,"");
    	buffer = (char *)malloc(bufsize);
    	num = recv(create_socket,buffer,bufsize,0);
    	buffer[num-1]='\0';
    	printf("Num of bytes received--->%d\nData received---> %s\n",num,buffer);
    	free(buffer);
      
    	/*---------Get command from terminal----*/
    	printf("Message to send: ");
    	buffer = (char *)malloc(bufsize);
    	gets(buffer);
    	num=strlen(buffer);

    	/*------Send command to server-------*/
    	num=send(create_socket,buffer,num+1,0);
 
    	/*--------free memory---------*/
    	free(buffer);
    	strcpy(buffer,"");

  	}while(1);

  	close(create_socket);
}
