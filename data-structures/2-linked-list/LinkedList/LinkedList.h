#ifndef __LinkedList_h__
#define __LinkedList_h__

void initial(struct node*);
void addList(struct node*, int);
struct node *addFront(struct node*, int);
void list(struct node*);
void deleteNode(struct node*, int);
void deleteFront(struct node*);
struct node *find(struct node*, int);
#endif
