#ifndef MY_HTABLE
#define MY_HTABLE


struct Node{
	int val;
	struct Node* next;
};


struct Node* ListInsert(struct Node *l, int key);
void ListFree(struct Node *l);


/* Hash table implementation making use of separate chaining */

struct HTable{
	struct Node **table;
	int size;
	int (*hashfn)(int key);
};


int HashFn(int key);
void HashInit(struct HTable *table, int sz, int (*hashfn)(int key));
void HashFree(struct HTable *table);
void HashInsert(struct HTable *table, int key);

/* Returns 1 if search hit, 0 otherwise */
int HashSearch(struct HTable *table, int key);

#endif
