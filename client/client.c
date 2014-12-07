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
#include <stdlib.h>
#include <stdio.h>
int main(int argc,char** argv)
{
	int sockfd,i,n;
	struct sockaddr_in servaddr,localaddr;
	struct register_data data_in,data_out;
	char tmp[100];
	if(argc < 3)
	{
		exit(1);
	}

	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,SERVER_ADDR,&servaddr.sin_addr);
	
	if(connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr)))
	{
		dumperror("connect error\n");
		return 1;
	}

	bzero(&data_out,sizeof(data_in));
	
/*Register begin*/

	if(!strcmp(argv[1],"register"))
	{
		ParceRegister(&data_out,argv[2]);
		write(sockfd,&data_out,sizeof(data_out));
		n=read(sockfd,&data_in,sizeof(data_in));
		printf("sizeof data_in:%d\n",sizeof(data_in));
		printf("received data %d\n",n);
		switch(data_in.result)
		{
			case NON_ALPHABET_NOT_ALLOWED_ERROR:
			case SAME_NAME_REGISTER_ERROR:
				printf("Please try again\n");
			case ALREADY_REGISTERED_ERROR:
				close(sockfd);
				return 1;
				break;
			case SUCCESS:
			{
				printf("Register SUCCESS\n");
				ParceReady(&data_out,argv[2]);
				write(sockfd,&data_out,sizeof(data_out));
				if(getsockname(sockfd,(struct sockaddr*)&localaddr,&n))
				{
					dumperror("getsockname error\n");
					return 1;
				}
				close(sockfd);
				sprintf(tmp,"%d",ntohs(localaddr.sin_port));
				
				execl("/sbin/iptables","iptables","-t","nat","-A","PREROUTING","-p","tcp","--dport",tmp,"-j","REDIRECT","--to-port",argv[3],NULL);

			}
			default: 
			printf("Not define\n");
			//execl("/bin/ls","ls","/home",NULL);
			return 1;
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
