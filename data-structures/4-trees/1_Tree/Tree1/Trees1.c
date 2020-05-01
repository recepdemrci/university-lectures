#include<stdio.h>
#include<stdlib.h>

typedef struct {
	int data;
	struct Node *left;
	struct Node *right;
}Node;


void preOrder(Node *node) {
	if (node) {
		printf("%d", node->data);
		preOrder(node->left);
		preOrder(node->right);
	}
}

void inOrder(Node *node) {
	if (node) {
		inOrder(node->left);
		printf("%d", node);
		inOrder(node->right);
	}
}

void postOrder(Node *node) {
	if (node) {
		postOrder(node->left);
		postOrder(node->right);
		printf("%d", node->data);
	}
}