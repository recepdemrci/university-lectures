#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node *prev;
	struct node *next;
};

struct node* addList(struct node *, int);
void list(struct node*);

int main(void) {
	

	struct node *root;
	root = (struct node*)malloc(sizeof(struct node));
	if (root == NULL) {	printf("< ERROR >");	return -1; }
	root->data = 4;
	root->next = NULL;
	root->prev = NULL;

	root = addList(root, 1);
	root = addList(root, 7);
	root = addList(root, 3);
	root = addList(root, 4);
	list(root);
	
	getch();
	return 0;
}


/* Verilen sayıyı ve linked listenin basını parametre olarak alıp sıralı olan listedeki yerine yerleştirmek*/
struct node* addList(struct node *root, int data) {
	struct node *p, *tmp;
	p = root;
	tmp = (struct node*)malloc(sizeof(struct node));
	if (tmp == NULL) { printf("< ERROR >"); exit(-2); }
	tmp->data = data;

	while ((p->data <= data) && p->next != NULL) {
		p = p->next;
	}
	if (p->data > data) {
		if (p == root) {
			tmp->prev = NULL;
			tmp->next = p;
			p->prev = tmp;
			root = tmp;
		}
		else {
			tmp->next = p;
			tmp->prev = p->prev;
			p->prev = tmp;
			tmp->prev->next = tmp;
		}
	}
	else {
		tmp->prev = p;
		tmp->next = NULL;
		p->next = tmp;
	}

	return root;
}


void list(struct node *root) {
	struct node *p;
	p = root;

	while (p->next) {
		printf("%d", p->data);
		p = p->next;
	}
	printf("%d", p->data);
}