#include "htable.h"
#include <stdlib.h>
#include <stdio.h>

/* Allocates memory for a new node and tacks it in front of
the given list */
struct Node* ListInsert(struct Node *l, int key)
{
	struct Node* ptr = malloc(sizeof(struct Node));
	ptr->val = key;
	ptr->next = l;
	return ptr;
}


void ListFree(struct Node *l)
{
	struct Node *cur;
	while(l){
		cur = l;
		l = l->next;
		free(cur);
	}
}


/* Copied this from somewhere */
int HashFn(int key)
{
	key ^= (key >> 20) ^ (key >> 12);
	key = key ^ (key >> 7) ^ (key >> 4);
	return key;
}


void HashInit(struct HTable *t, int sz, int (*hashfn)(int key))
{
	t->size = sz;
	t->table = malloc(sz * sizeof(struct Node *));
	t->hashfn = hashfn;

	int i;
	for(i = 0; i < sz; ++i){
		t->table[i] = NULL;
	}
}



void HashFree(struct HTable *t)
{
	int i;
	for(i = 0; i < t->size; ++i){
		if(t->table[i]){
			ListFree(t->table[i]);
		}
	}

	free(t->table);
}


void HashInsert(struct HTable *t, int key)
{
	struct Node *curNode;
	int hashVal = t->hashfn(key) % t->size;

	//printf("key = %d, hashVal = %d\n", key, hashVal);
	/* Check if it's a previously inserted value */
	curNode = t->table[hashVal];
	while(curNode != NULL){
		if(curNode->val == key){
			return;
		}
		curNode = curNode->next;
	}

	t->table[hashVal] = ListInsert(t->table[hashVal], key);
}



int HashSearch(struct HTable *t, int key)
{
	int hashVal = t->hashfn(key) % t->size;
	struct Node *curNode;

	curNode = t->table[hashVal];
	while(curNode != NULL){
		if(curNode->val == key){
			return 1;
		}
		curNode = curNode->next;
	}

	return 0;
}
