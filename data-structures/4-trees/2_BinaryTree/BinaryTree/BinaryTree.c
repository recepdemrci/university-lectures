#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct {
	int value;
	Node *right;
	Node *left;
}Node;

void insert(Node *root, int value) {
	Node *current, *newNode, *parent;

	newNode = (Node *)malloc(sizeof(Node));
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;

	if (!root) {
		root = newNode;
	}
	else {

		bool stop = false;
		current = root;

		while (!stop) {

			parent = current;
			if (current->value > value) {	
				current = current->left;
				if (current == NULL) {
					parent->left = newNode;
					stop = true;
				}
			
			}
			else {
				current = current->right;
				if (current == NULL) {
					parent->right = newNode;
					stop = true;
				}
			}

		}
	
	}
	
}

Node *insert2(Node *root, int value) {
	
	if (root == NULL) {
		root = (Node*)malloc(sizeof(Node));
		return root;
	}
	if (value < root->value) {
		root->right = insert2(root->left, value);			//YANLIS OLABILIR
	}
	else if (value > root->value) {
		root->right = insert2(root->right, value);			// YANLIS OLABILIR
	}

	return root;

}

Node *search(Node *root, int value) {
	Node *current;
	current = root;

	bool stop = false;
	while ( (current->value != value) && (current != NULL) ) {

		if (current->value > value) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	return current;
}

Node *searchMin(Node *root) {
	Node *current;
	current = root;

	while (current->left) {
		current = current->left;
	}
	return current;
}

Node *delete(Node *root, int value) {
	Node *tmp;
	
	if (root == NULL) {
		return root;
	}

	if (value < root->value) {									//--------------------------------------------
		root->left = delete(root->left, value);					// Bu alanda aslında silinecek olan değeri
	}															// bulmaya çalışıyosun
	else if (value > root->value) {								//
		root->right = delete(root->right, value);				//
	}															//--------------------------------------------
	else {
		
		if (root->left == NULL) {
			tmp = root->right;
			free(root);
			return tmp;	
		}
		else if (root->right == NULL) {
			tmp = root->left;
			free(root);
			return tmp;
		}
		else {
			tmp = searchMin(root->right);
			root->value = tmp->value;
			root->right = del(root->right, tmp->value);
		}		
		return root;
	}

}



