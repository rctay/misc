#ifndef MY_HTABLE
#define MY_HTABLE


struct Node{
	int val;
	struct Node* next;

	/* The below 2 fields are for testing purposes only.
	total - total number of times this entry appears
	counter - number of times it's counted */
	int total;
	int counter;
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
struct HTable *HashNew(int sz, int (*hashfn)(int key));
void HashFree(struct HTable *table);
void HashInsert(struct HTable *table, int key);

/* Returns 0 if search hit, -1 otherwise */
int HashSearch(struct HTable *table, int key);

/* Same as HashSearch, but increments the counter for 
search hits */
int HashUpdateSearch(struct HTable *t, int key);


/* Checks each node to see if counter == total
Returns 0 if it's the case
Returns -1 otherwise */
int HashCheck(struct HTable *t);

/* Resets the counter in each node */
void HashCntReset(struct HTable *t);

#endif
