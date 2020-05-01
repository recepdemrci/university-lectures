#include<stdio.h>
#include<stdlib.h>


// MinHeapTree Example:
//---------------------


void swap(int *HeapTree, int childInd, int parentInd) {			// childInd ve parentInd : its are indis of which we wanna change
	int tmp;

	tmp = HeapTree[childInd];
	HeapTree[childInd] = HeapTree[parentInd];
	HeapTree[parentInd] = tmp;
}

int findSmallestChild(int *HeapTree, int n, int parent) {

	if (n > (2 * parent + 2)) {												// Parent has 2 child

		if (HeapTree[2 * parent + 1] < HeapTree[2 * parent + 2]) {
			return (2 * parent + 1);
		}
		else {
			return (2 * parent + 2);
		}

	}
	else if (n == (2 * parent + 2)) {										// Parent has 1 child

		return (2 * parent + 1);
	}
	else {
		return 0;															// Parent has no child
	}
}

// insert function is also include upHeap 
void insert(int *HeapTree, int *n, int value) {					// n : size of the Heap
	int i, parent;

	HeapTree[*n] = value;										// add value to end of the Tree(Left justified)
	i = *n;	
	parent = (i - 1) / 2;
	(*n)++;

	while ((HeapTree[i] < HeapTree[parent]) && (i >= 1)) {			
		swap(HeapTree, i, parent);								 
		i = parent;
		parent = (i - 1) / 2;
	}
}

void downHeap(int *HeapTree, int n) {
	int tmp, i, childIndis;

	i = 0;
	childIndis = findSmallestChild(HeapTree, n, i);
	while ((childIndis != 0) && (HeapTree[i] > HeapTree[childIndis])) {
		swap(HeapTree, childIndis, i);
		i = childIndis;
		childIndis = findSmallestChild(HeapTree, n, i);
	}
}

void removeMin(int *HeapTree, int *n) {							// remove minimum number of the HeapTree which is also root
	int min;													
	
	if ((*n) == 0) {
		exit(-1);
	}

	(*n)--;														// 1- Decrement size of HeapTree
	min = HeapTree[0];											
	HeapTree[0] = HeapTree[*n];									// 2- Swap the first and the last elements
	HeapTree[*n] = min;
	
	downHeap(HeapTree, (*n) );									// 3- Heapify to down (downHeap)		
}
	
void sortHeap(int *HeapTree, int n) {							// Sorting array in a decreasing order with HeapSort algorithm
	int size = n - 1;

	for (int i = 0; i <= n - 1; i++) {
		swap(HeapTree, 0, size);
		//(size--);
		downHeap(HeapTree, size);
		(size--);
	}
}

void main(void) {

	static int HeapTree[50];
	int size = 0;

	insert(HeapTree, &size, 2);
	insert(HeapTree, &size, 9);
	insert(HeapTree, &size, 7);
	insert(HeapTree, &size, 6);
	insert(HeapTree, &size, 5);
	insert(HeapTree, &size, 8);

	printf("%d", HeapTree[0]);
	printf("%d", HeapTree[1]);
	printf("%d", HeapTree[2]);
	printf("%d", HeapTree[3]);
	printf("%d", HeapTree[4]);
	printf("%d", HeapTree[5]);


	printf("\n");
	removeMin(HeapTree, &size);
	printf("%d", HeapTree[0]);
	printf("%d", HeapTree[1]);
	printf("%d", HeapTree[2]);
	printf("%d", HeapTree[3]);
	printf("%d", HeapTree[4]);

/*
	printf("\n");
	swap(HeapTree, 1, 2);
	printf("%d", HeapTree[0]);
	printf("%d", HeapTree[1]);
	printf("%d", HeapTree[2]);
	printf("%d", HeapTree[3]);
	printf("%d", HeapTree[4]);
	*/

	printf("\n");
	sortHeap(HeapTree, size);			
	printf("%d", HeapTree[0]);
	printf("%d", HeapTree[1]);
	printf("%d", HeapTree[2]);
	printf("%d", HeapTree[3]);
	printf("%d", HeapTree[4]);



	
	getch();
}