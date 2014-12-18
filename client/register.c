#include <string.h>
#include <stdio.h>
#include "../common/common.h"
#include "client.h"
void ParceRegister(struct register_data *data_out,char *name)
{
	data_out->type = REGISTER;
	strcpy(data_out->name,name);
	return ;
}

void ParceRequest(struct register_data* data_out,char *name)
{
	data_out->type = REQUEST;
	strcpy(data_out->name,name);
	return ;
}

void ParceReady(struct register_data* data_out, char *name)
{
	data_out->type = READY;
	return ;
}

void ResultRequest(struct register_data* data_in)
{
	switch(data_in->result)
	{
		case SUCCESS:
		printf("%s:%d\n",inet_ntoa(data_in->addr),ntohs(data_in->port));
		break;
		case NOT_FOUND_REQUEST_ERROR:
		printf("The name not found!\n");
		break;
		default:
		printf("Error not define!\n");
		break;
	}
}

