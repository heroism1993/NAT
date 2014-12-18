#include <stdio.h>
#include "../common/common.h"
#include "../common/config.h"
#include "../common/error.h"
#include "../common/trie.h"
#include "server.h"
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void process(void*);
struct data_pack{
	int connfd;
	struct sockaddr_in* clientaddr;
};

pthread_mutex_t thread_data;

int main()
{
	int socketfd,connfd,tmp,n;
	pid_t childpid;
	pthread_t threadid;
	struct sockaddr_in cliaddr,servaddr;
	struct register_data data_in,data_out;
	struct data_pack pack;
	char buf[1024];

	init();
	pthread_mutex_init(&thread_data,NULL);

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

		pthread_mutex_lock(&thread_data);
		pack.connfd = connfd;
		pack.clientaddr = &cliaddr;
		pthread_create(&threadid,NULL,process,(void *)&pack);
		/*
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
						dump_register_data(&data_in);
						register_node(&data_in,&data_out,getpid());
						dump_register_data(&data_out);
						break;
				case REQUEST:
						printf("request:%s\n",data_in.name);
					
						dump_register_data(&data_in);
						request_node(&data_in,&data_out);
						dump_register_data(&data_out);
						break;
				case DEREGISTER:
						data_in.addr=cliaddr.sin_addr;
						data_in.port=cliaddr.sin_port;
						dump_register_data(&data_in);
						deregister_node(&data_in,&data_out);
						dump_register_data(&data_out);
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
		}*/
	}
	
}

void process(void* data)
{
	struct register_data data_in,data_out;
	struct data_pack* pack = (struct data_pack*)data;
	int connfd = pack->connfd,n;
	struct sockaddr_in cliaddr;
	memcpy(&cliaddr,pack->clientaddr,sizeof(cliaddr));
	pthread_mutex_unlock(&thread_data);

	printf("My thread id:%d\n",pthread_self());
	printf("connect created with %s:%d\n",inet_ntoa(cliaddr.sin_addr),htons(cliaddr.sin_port));
	bzero(&data_in,sizeof(data_in));
	n=read(connfd,&data_in,sizeof(data_in));
	switch(data_in.type)
	{
		case REGISTER:
				printf("begin to register:%s with pid:%d\n",data_in.name,getpid());
				data_in.addr=cliaddr.sin_addr;
				data_in.port=cliaddr.sin_port;
				dump_register_data(&data_in);
				register_node(&data_in,&data_out,getpid());
				dump_register_data(&data_out);
				break;
		case REQUEST:
				printf("request:%s\n",data_in.name);
			
				dump_register_data(&data_in);
				request_node(&data_in,&data_out);
				dump_register_data(&data_out);
				break;
		case DEREGISTER:
				data_in.addr=cliaddr.sin_addr;
				data_in.port=cliaddr.sin_port;
				dump_register_data(&data_in);
				deregister_node(&data_in,&data_out);
				dump_register_data(&data_out);
				break;
		default:
				break;
	}
	write(connfd,&data_out,sizeof(data_out));
	if(data_in.type == REGISTER && data_out.type == SUCCESS)
	{
		n=read(connfd,&data_in,sizeof(data_in));
		if(data_in.type == READY)
		{
			printf("READY\n");
			sleep(10);
			close(connfd);
			while(1)
			{
				connfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
				connect(connfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
				sleep(30);
				close(connfd);
				sleep(30);
			}
		}
	}
	close(connfd);
}
