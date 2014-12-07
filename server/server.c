#include <stdio.h>
#include "../common/common.h"
#include "../common/config.h"
#include "../common/error.h"
#include "server.h"
#include <unistd.h>

int main()
{
	int socketfd,connfd,tmp,n;
	pid_t childpid;
	struct sockaddr_in cliaddr,servaddr;
	struct register_data data_in,data_out;
	char buf[1024];
	socketfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socketfd == -1)
	{
		dumperror("socket created error\n");
		exit(1);
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	if(bind(socketfd,(struct sockaddr *)&servaddr,sizeof(servaddr)))
	{
		dumperror("bind error\n");
		exit(1);
	}
	if(listen(socketfd,100))
	{
		dumperror("listen error\n");
		exit(1);
	}

	while(1)
	{
		tmp = sizeof(cliaddr);
		if((connfd = accept(socketfd,(struct sockaddr *) &cliaddr,&tmp))<0) continue;
		if((childpid = fork())==0)
		{
			printf("connect created with %s:%d\n",inet_ntoa(cliaddr.sin_addr),htons(cliaddr.sin_port));
			close(socketfd);
			bzero(&data_in,sizeof(data_in));
			n=read(connfd,&data_in,sizeof(data_in));
			switch(data_in.type)
			{
				case REGISTER:
						printf("begin to register:%s with pid:%d\n",data_in.name,getpid());
						data_in.addr=cliaddr.sin_addr;
						data_in.port=cliaddr.sin_port;
						register_node(&data_in,&data_out,getpid());
						break;
				case REQUEST:
						request_node(&data_in,&data_out);
						break;
				case DEREGISTER:
						data_in.addr=cliaddr.sin_addr;
						data_in.port=cliaddr.sin_port;
						deregister_node(&data_in,&data_out);
						break;
				default:
						break;
			}
			write(connfd,&data_out,sizeof(data_out));
			if(data_in.type == REGISTER)
			{
				n=read(connfd,&data_in,sizeof(data_in));
				if(data_in.type == READY)
				{
					sleep(10);
					close(connfd);
					while(1)
					{
						socketfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
						connect(socketfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
						sleep(30);
						close(socketfd);
						sleep(30);
					}
				}
			}
		}
	}
	
}
