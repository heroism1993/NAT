/*
Usage
client [options] [name] [port]

options:
	register name port
	request name
*/

#include "../common/common.h"
#include "../common/error.h"
#include "../common/config.h"
#include "client.h"
#include <string.h>
int main(int argc,char** argv)
{
	int sockfd,i,n;
	struct sockaddr_in servaddr;
	struct register_data data_in,data_out;
	if(argc!=3)
	{
		exit(1);
	}

	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,SERVER_ADDR,&servaddr.sin_addr);
	
	connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr));

	bzero(&data_out,sizeof(data_in));
	
/*Register begin*/

	if(!strcmp(argv[1],"register"))
	{
		ParceRegister(&data_out,argv[2]);
		write(sockfd,&data_out,sizeof(data_out));
		n=read(sockfd,&data_in,sizeof(data_in));
		switch(data_in.result)
		{
			case SAME_NAME_REGISTER_ERROR:
				printf("Please try again\n");
			case ALREADY_REGISTERED_ERROR:
				close(sockfd);
				return 1;
				break;
			case SUCCESS:
			default: return 1;
		}
	}

	/*request begin*/
	else if(!strcmp(argv[1],"request"))
	{
		ParceRequest(&data_out,argv[2]);
		write(sockfd,&data_out,sizeof(data_out));
		n=read(sockfd,&data_in,sizeof(data_in));
		ResultRequest(&data_in);
	}
	/*
	for(i=0;i<100;i++)
	{	
		write(sockfd,&i,sizeof(i));
		n=read(sockfd,&i,sizeof(i));
		printf("%d\n",i);

	}*/
	close(sockfd);
	
}
