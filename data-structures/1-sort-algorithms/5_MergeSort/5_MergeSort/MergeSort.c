#include <stdio.h>
#include <stdlib.h>

void Merge(int*,int, int, int);


void MergeSort(int *A, int left, int right) {
		
	if (left < right) {
		int middle = (right + left) / 2;
		
		MergeSort(A, left, middle);
		MergeSort(A, middle + 1, right);
		Merge(A, left, middle, right);
	}	
}

void Merge(int *A, int left, int middle, int right) {
	int i = 0, j = 0, k = left;
	
	int size1 = (middle - left + 1);
	int size2 = (right - middle);
	
	int *B = (int*)malloc(sizeof(int)*size1);
	int *C = (int*)malloc(sizeof(int)*size2);
	
	for (int l = 0; l < size1; l++) {
		B[l] = A[left+l];
	}
	for (int l = 0; l < size2; l++) {
		C[l] = A[middle+1+l];
	}

	while (i < size1 && j < size2) {
		if (B[i] < C[j]) {
			A[k] = B[i];
			i++;
		}
		else {
			A[k] = C[j];
			j++;
		}
		k++;
	}

	if (i == size1) {
		while (j < size2) {
			A[k] = C[j];
			j++;
			k++;
		}
	}
	else {
		while (i < size1) {
			A[k] = B[i];
			i++;
			k++;
		}
	}
}

void main(void) {

	int A[] = { 2,3,4,1,5,11,0,9,7,8,6,10,15,12,14,13 };

	MergeSort(A,0,15);
	for (int i = 0; i < 16; i++) {
		printf("%d\n", A[i]);
	}
	getch();
}