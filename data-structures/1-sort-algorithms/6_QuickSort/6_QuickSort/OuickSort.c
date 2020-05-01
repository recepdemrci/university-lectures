#include <stdio.h>
#include <stdlib.h>

int partition(int*, int, int, int);
void swap(int *, int, int);


void quickSort(int *array, int left, int right) {
	if (left >= right) { return; }

	int pivot = array[(left + right) / 2];
	int index = partition(array, left, right, pivot);
	quickSort(array, left, index - 1);
	quickSort(array, index, right);
}

int partition(int *array, int left, int right, int pivot) {
	while (left <= right) {
		
		while (array[left] < pivot) {
			left++;
		}

		while (array[right] > pivot) {
			right--;
		}

		if (left <= right) { 
			swap(array, left, right);
			left++;
			right--;
		}
	}
	return left;
}

void swap(int *array, int left, int right) {
	int tmp = array[left];
	array[left] = array[right];
	array[right] = tmp;
}

void main(void) {
	int array[] = { 3,5,0,6,2,1,9,7,8,4 };

	quickSort(array, 0, 9);

	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", array[i]);
	}

	getch();
}