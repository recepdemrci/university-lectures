#include<stdio.h>
#include<stdlib.h>
#include"LinkedList.h"


struct node {
	int data;
	struct node *next;
};

void initial(struct node*);
void addList(struct node*, int);
struct node *addFront(struct node*, int);
void list(struct node*);
void deleteNode(struct node*, int);
void deleteFront(struct node*);
struct node *find(struct node*, int);

/*
int main() {

	int num;
	struct node *root, *newRoot, *tmp;

	root = malloc(sizeof(struct node));
	if (root == NULL) {
		printf("ERROR");
		return 1;
	}
	root->data = 1;
	root->next = NULL;


	printf("listeye eklencek sayiyi giriniz:");
	scanf("%d", &num);
	addList(root, num);
	newRoot = addFront(root, num);
	list(root);
	list(newRoot);


	printf("listeden silinecek sayiyi giriniz:");
	scanf("%d", &num);
	if (newRoot->data == num) {
		tmp = newRoot->next;
		free(newRoot);
		newRoot = tmp;
	}
	else {
		deleteNode(newRoot, num);
	}
	list(newRoot);

	/*
		int num = 1;
		struct node *root, *newnode;
		root = malloc(sizeof(struct node));
		if(root == NULL){
			printf("ERROR");
			return 1;
		}
		printf("dizi elemanlarini giriniz:");
		scanf("%d",&root->data);
		root->next == NULL;

		while(num){
			scanf("%d",&num);
			newnode = find(root , num);
			if(newnode){
				addList(newnode , num);
			}
			else{
				root = addFront(newnode, num);
			}
		}
	*//*
	return 0;
}*/



void initial(struct node *root) {
	root = NULL;
}

void addList(struct node *nodegelen, int num) {
	
	struct node *p;
	p = malloc(sizeof(struct node));
	p->data = num;


	if (nodegelen->next == NULL)
	{
		nodegelen->next = p;
		p->next = NULL;
	}
	else {
		p->next = nodegelen->next;
		nodegelen->next = p;
	}

	/*	while(p->next != NULL){
			p = p->next;
		}
		p->next = malloc(sizeof(struct node));
		p->next->data = num;
		p->next->next = NULL;
	*/
}

struct node *addFront(struct node* root, int num) {
	struct node *p;
	p = malloc(sizeof(struct node));
	p->next = root;
	p->data = num;

	return p;
}

void list(struct node *root) {
	struct node *p;
	p = root;

	while (p->next) {
		printf("%d", p->data);
		p = p->next;
	}
	printf("%d\n", p->data);
}

//basta olmasını kontrol et
void deleteNode(struct node* newRoot, int num) {

	struct node *p, *tmp;
	p = newRoot, tmp = NULL;

	while ((p->data != num) && (p->next != NULL)) {
		tmp = p;
		p = p->next;
	}
	if (p->data == num) {
		if (p == newRoot) {
			deleteFront(newRoot);
		}
		else {
			tmp->next = p->next;
			free(p);
		}
	}
	else {
		printf("silinecek sayi bulunamadi");
	}
}

void deleteFront(struct node* root) {
	struct node *tmp;

	tmp = root;
	root = root->next;
	free(tmp);
}

struct node *find(struct node *root, int num) {
	struct node *p, *tmp = NULL;
	p = root;

	while (p->data < num  &&  p->next != NULL) {
		tmp = p;
		p = p->next;
	}
	if (p->data < num  && tmp != NULL) {
		return p;
	}
	else {
		return tmp;
	}

}
