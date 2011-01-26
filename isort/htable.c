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

	ptr->total = 1;
	ptr->counter = 0;
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


struct HTable *HashNew(int sz, int (*hashfn)(int key))
{
	struct HTable *t;
	t = malloc(sizeof(struct HTable));

	if(t == NULL){
		return NULL;
	}

	t->table = malloc(sz * sizeof(struct Node *));
	if(t->table == NULL){
		free(t);
		return NULL;
	}

	t->size = sz;
	t->hashfn = hashfn;

	int i;
	for(i = 0; i < sz; ++i){
		t->table[i] = NULL;
	}

	return t;
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
	free(t);
}


void HashInsert(struct HTable *t, int key)
{
	struct Node *curNode;
	int hashVal = t->hashfn(key) % t->size;

	/* Check if it's a previously inserted value */
	curNode = t->table[hashVal];
	while(curNode != NULL){
		if(curNode->val == key){
			/* Below line is also for testing purposes */
			++curNode->total;
			return;
		}
		curNode = curNode->next;
	}

	t->table[hashVal] = ListInsert(t->table[hashVal], key);
}



/* Returns 0 on search hit, -1 on search miss */
int HashSearch(struct HTable *t, int key)
{
	int hashVal = t->hashfn(key) % t->size;
	struct Node *curNode;

	curNode = t->table[hashVal];
	while(curNode != NULL){
		if(curNode->val == key){
			return 0;
		}
		curNode = curNode->next;
	}

	return -1;
}


/* Same as above function, but increments the counter
entry in the node if it's a search hit */
int HashUpdateSearch(struct HTable *t, int key)
{
	int hashVal = t->hashfn(key) % t->size;
	struct Node *curNode;

	curNode = t->table[hashVal];
	while(curNode != NULL){
		if(curNode->val == key){
			++curNode->counter;
			return 0;
		}
		curNode = curNode->next;
	}

	return -1;
}


/* Checks each node to see if counter == total
Returns 0 if it's the case.
Returns -1 otherwise
*/
int HashCheck(struct HTable *t)
{
	int i;
	struct Node *curNode;

	for(i = 0; i < t->size; ++i){
		curNode = t->table[i];
		while(curNode != NULL){
			if(curNode->counter != curNode->total){
				return -1;
			}
			curNode = curNode->next;
		}
	}

	return 0;
}



/* Resets the counter entries in nodes to 0 */
void HashCntReset(struct HTable *t)
{
	int i;
	struct Node *curNode;

	for(i = 0; i < t->size; ++i){
		curNode = t->table[i];
		while(curNode != NULL){
			curNode->counter = 0;
			curNode = curNode->next;
		}
	}
}
