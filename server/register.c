#include "../common/common.h"
#include "../common/trie.h"
#include "../common/error.h"
#include "server.h"
#include <stdlib.h>
/*
	BY gaochao
	Use the heap memory.
	ADD LOCK!!!!!!!
*/
struct trie_node *root;
//indexed by name
struct stored_data{
	struct in_addr addr;
	in_port_t port;
	pid_t pid;
};

void init()
{
	root=create();
}
void kill(pid_t pid)
{

}
/**
	valid field:
		data_in->name,addr,port

	field to be filled:
		data_out->result

	valid result:
		see common/common.h

	TODO:
		1.organize the struct stored_data.
		2.insert into dataset(PS ERROR)


**/
void register_node(struct register_data *data_in,struct register_data* data_out,pid_t pid)
{
	int t;
	struct stored_data *tmp,* s=malloc(sizeof(struct stored_data));
	s->addr=data_in->addr;
	s->port=data_in->port;
	s->pid=pid;
	switch(t=(struct stored_data*)insert(data_in->name,(void*)s,root))
	{
		case ROOT_NULL:
			dumperror("ROOT INIT ERROR!\n");
			break;
		case INSERT_SUCCESS:
			data_out->result = SUCCESS;
			break;
		default:
			{
				tmp=(struct stored_data*)(((struct trie_node*)t)->data);
				if(tmp->addr.s_addr==s->addr.s_addr&&tmp->port==s->port)
				{
					data_out->result=ALREADY_REGISTERED_ERROR;
				}
				else data_out->result = SAME_NAME_REGISTER_ERROR;
				kill(pid);
			}


	}
}	

/**
	valid field:
		data_in->name

	field to be filled:
		data_out->addr,port,result

	valid result:
		see common/common.h

**/
void request_node(struct register_data* data_in, struct register_data* data_out)
{
	struct trie_node *t;
	struct stored_data *s;
	t=search(data_in->name,root);
	if(!t)
	{
		data_out->result=NOT_FOUND_REQUEST_ERROR;
		return ;
	}
	s=(struct stored_data*)(t->data);
	data_out->addr = s->addr;
	data_out->port = s->port;
	data_out->result = SUCCESS;
}


/**
	valid field:
		data_in->name,addr,port

	field to be filled:
		data_out->result

	valid result:
		see common/common.h

	TODO:
		1.search the stored data with 'name'
		2.check the stored data with 'addr port'
		3.if valid ,remove the stored data
		4.kill the pid
**/
void deregister_node(struct register_data* data_in,struct register_data*data_out)
{
	struct stored_data *s;
	struct trie_node *t;
	t=search(data_in->name,root);
	if(!t)
	{
		data_out->result=NAME_NOT_FOUND_ERROR;
		return ;
	}
	s=(struct stored_data*)(t->data);

	if(!(data_in->addr.s_addr==s->addr.s_addr&&data_in->port==s->port))
	{
		data_out->result=SOURCE_NOT_MATCH_ERROR;
	}
	kill(s->pid);
	t->data=NULL;
	data_out->result=SUCCESS;
}
