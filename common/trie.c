#include "trie.h"
#include "common.h"
#include <string.h>

static pthread_mutex_t trie_lock;

struct trie_node* create()
{
	struct trie_node* node=malloc(sizeof(struct trie_node));
	bzero(node,sizeof(struct trie_node));
	return node;
}

struct trie_node* search(char* name,struct trie_node* root)
{
	struct trie_node* result = root;
	char* p=name;
	if(!root)return NULL;
	pthread_mutex_lock(&trie_lock);
	while(*p!=0)
	{
		result = result->next[*p-'a'];
		if(result==NULL)
			break;
		p++;
	}
	pthread_mutex_unlock(&trie_lock);
	return result;
}

int insert(char *name,void *data,struct trie_node* root)
{
	struct trie_node* result=root;
	char* p=name;
	if(!root) return ROOT_NULL;
	pthread_mutex_lock(&trie_lock);
	while(*p!=0)
	{
		if(result->next[*p-'a']==0)
		{
			result->next[*p-'a']=create();
		}
		result = result->next[*p-'a'];
		p++;
	}
	if(result->data)
	{
		pthread_mutex_unlock(&trie_lock);
		return (int)result->data;
	}
	result->data=data;
	pthread_mutex_unlock(&trie_lock);
	return INSERT_SUCCESS;
}

