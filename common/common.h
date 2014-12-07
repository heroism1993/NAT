#include <sys/socket.h>
#include <arpa/inet.h>

struct register_data{
	int type;
	char name[20];
	struct in_addr addr;
	in_port_t port;
	int result;	
};

enum{
	REGISTER = 0x00,
	ACK_REGISTER = 0x01,
	REQUEST = 0x02,
	ACK_REQUEST = 0x04,
	DEREGISTER = 0x08,
	ACK_DEREGISTER = 0x10,
	READY = 0x20,
	TYPE_NOT_DEFINE = 0x100
};

enum{
	//for all
	SUCCESS = 0x00,
	NON_ALPHABET_NOT_ALLOWED_ERROR = 0x2000,

	//for register
	SAME_NAME_REGISTER_ERROR = 0x01,
	ALREADY_REGISTERED_ERROR = 0x02,

	//for request
	NOT_FOUND_REQUEST_ERROR = 0x10,

	//for deregister
	SOURCE_NOT_MATCH_ERROR = 0x100,
	
	ERROR_NOT_DEFINE = 0x1000
};
