#include "error.h"
#include "common.h"
#include <stdio.h>

int dumperror(char * data)
{
	printf("%s\n",data);
	return 1;
}


void dump_register_data(struct register_data* data)
{
	printf("Type:%d\n",data->type);
	printf("Name:%s\n",data->name);
	printf("Result:%d\n",data->result);
}
