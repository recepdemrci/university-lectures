#include <stdio.h>

#define N 6

int main(void) {
	int i, j, key;
	int a[] = { 8,2,4,9,3,6 };

	for (j = 1; j < N; j++) {
		key = a[j];
		i = j - 1;

		while (i >= 0 && key < a[i]) {
			a[i + 1] = a[i];
			a[i] = key;
			i--;
		}
	}

	for (i = 0; i < N; i++) {
		printf("%d", a[i]);
	}

	getch();
	return 0;
}