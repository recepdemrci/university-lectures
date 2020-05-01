#include <stdio.h>

#define N 6

int main(void) {

	int tmp, i, j, mindx;
	int a[] = { 2,1,4,5,6,3 };

	for (i = 0; i < N; i++) {
		mindx = i;

		for (j = i + 1; j < N; j++) {
			if (a[j] < a[mindx]) {
				mindx = j;
			}
		}
		tmp = a[i];
		a[i] = a[mindx];
		a[mindx] = tmp;
	}

	for (i = 0; i < N; i++) {
		printf("%d", a[i]);
	}

	getch();
	return 0;
}