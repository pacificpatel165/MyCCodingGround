/* server.c */
 
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>      
#include<fcntl.h>
#include<string.h>
 
int main()
{
	int cont,create_socket,new_socket,addrlen;
	int bufsize = 10024;
	char buf[10024]={'\0'};	
	char *buffer = (char *)malloc(bufsize);
	struct sockaddr_in address;
	int i32FileSize2;
	int fp,num,flag =1; 

	//printf("\x1B[2J");
	//printf("%c[2J",27);

	if ((create_socket = socket(AF_INET,SOCK_STREAM,0)) > 0)
		printf("The socket was created\n");

  	address.sin_family = AF_INET;
  	address.sin_addr.s_addr = INADDR_ANY;
  	address.sin_port = htons(15000);

  	if (bind(create_socket,(struct sockaddr *)&address,sizeof(address))== 0)
  		printf("Binding Socket\n");

  	listen(create_socket,3);

  	addrlen = sizeof(struct sockaddr_in);
  	new_socket = accept(create_socket,(struct sockaddr *)&address,&addrlen);
  	if (new_socket > 0)
  	{
  		printf("The Client %s is connected...\n",inet_ntoa(address.sin_addr));
  		for(cont=1;cont<5000;cont++)
  			printf("\x7");
  	}
  	do
  	{
    	if(flag == 1)
    	{
      		strcpy(buf,"Serev is waiting for cmds");
      		num = strlen(buf);
      		buf[num + 1] = '\0';
      		send(new_socket,buf,num,0);
    	}
	    /*---receive message from client--------*/
    	flag = 0;
    	buffer = (char *)malloc(bufsize);
    	recv(new_socket,buffer,bufsize,0);

    	/*-----open the file-------*/
    	fp = open("txt.txt",O_RDWR|O_CREAT|O_TRUNC,0777);  
    
    	/*------Execute command from client-------*/
    	system(buffer);

    	/*---Read data from file-----------*/
    	i32FileSize2=lseek(fp,0L,SEEK_END);
    	lseek(fp,0L,SEEK_SET);
    	num = read(fp,buf,i32FileSize2);
    	buf[num] = '\0';
    
    	/*--------Send data to client side-----*/
    	num=send(new_socket,buf,num,0);
    
    	/*-----free memory and close file-------*/
    	free(buffer);
    	strcpy(buffer,"");
    	strcpy(buf,"");
    	close(fp);

   	}while(1);

   	close(new_socket);
   	close(create_socket);
}
