#include <stdlib.h>
#include <pthread.h>

struct trie_node{
	void *data;
	struct trie_node* next[26];
};

enum{
	//ALREADY_EXISTS = 0x00,
	ROOT_NULL =0x01,
	INSERT_SUCCESS = 0x00
};
struct trie_node* create();
struct trie_node* search(char* name, struct trie_node* root);
int insert(char* name,void* node,struct trie_node* root);
