#include "../common/common.h"
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
//indexed by name
typedef struct stored_data{
	struct in_addr addr;
	int port;
	pid_t pid;
}stored_data_;
typedef struct data_tree{
	char name[20];
	struct data_tree *next[26];
	stored_data_* data;		
}data_tree_;
data_tree_ *head;
pthread_mutex_t  lock;
void init()
{
	printf("Init root.\n");
	head=(data_tree_*)malloc(sizeof(data_tree_));	
	pthread_mutex_init(&lock, NULL);
}
void register_node(struct register_data *data_in,struct register_data* data_out,pid_t pid)
{
	//need to be locked
	//successfully register
	char *name=data_in->name;
	int i;
	data_tree_* s=head;	
	pthread_mutex_lock(&lock);	
	printf("Begin to register %s.\n",data_in->name);
	for(i=0;name[i]!=0;i++)
	{       int index;
        	if(name[i]>=97)
          		index=name[i]-97;
       	 	else
           		index=name[i]-65;
		if(s->next[index]==NULL)
		{	
			data_tree_* p=(data_tree_*)malloc(sizeof(data_tree_));
			p->data=NULL;
			s->next[index]=p;
			s=s->next[index];
		}
		else
			s=s->next[index];	
	}
	if(s->data==NULL)
	{       printf("enen ");
		stored_data_ *p=(stored_data_*)malloc(sizeof(stored_data_));
		p->addr=data_in->addr;
		p->port=data_in->port;
		p->pid=pid;
		s->data=p;
		data_out->result=SUCCESS;
	}
	//other failed situation
	else
	{	
		data_out->result=SAME_NAME_REGISTER_ERROR;
	}
	pthread_mutex_unlock(&lock);
}
void request_node(struct register_data* data_in, struct register_data* data_out)
{
	//if found
	char *name=data_in->name;
	int i;
	int flag=1;
	data_tree_ *s=head;
	pthread_mutex_lock(&lock);
	printf("Begin to search %s.\n",data_in->name);
	for(i=0;name[i]!=0;i++)
	{    				
        	if(name[i]>=97)
			s=s->next[name[i]-97];
       		else
			s=s->next[name[i]-65];

		if(s==NULL)
		{	
			flag=0;
			break;
		}	
	}
	if(flag==1&&s->data!=NULL)
	{
		data_out->addr=s->data->addr;
		data_out->port=s->data->port;
		data_out->result=SUCCESS;
	}
	//if not found
	else
	{
		data_out->result=NOT_FOUND_REQUEST_ERROR;
	}
	
	pthread_mutex_unlock(&lock);
}
void deregister_node(struct register_data* data_in,struct register_data* data_out)
{	
	
	int i;
	data_tree_ *s=head;
	char *name=data_in->name;
	pthread_mutex_lock(&lock);
	for(i=0;name[i]!=0;i++)
	{
		if(s==NULL)
			break;
		if(name[i]>=97)
			s=s->next[name[i]-97];
		else
			s=s->next[name[i]-65];
	}
	pthread_mutex_unlock(&lock);
	if(s!=NULL&&s->data!=NULL)
	{    //if node match
		if(/*data_in->addr==s->data->addr&&*/data_in->port==s->data->port)//......................
		{
			s->data=NULL;
			data_out->result=SUCCESS;
		}
		else//if node not match
			data_out->result=SOURCE_NOT_MATCH_ERROR;
	}
	else //if not exist
		data_out->result=ERROR_NOT_DEFINE;	
}

