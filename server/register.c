#include "../common/common.h"
/*
	BY gaochao
	Use the heap memory.
	ADD LOCK!!!!!!!
*/

//indexed by name
struct stored_data{
	struct in_addr addr;
	in_port_t port;
	pid_t pid;
};

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
	
}
