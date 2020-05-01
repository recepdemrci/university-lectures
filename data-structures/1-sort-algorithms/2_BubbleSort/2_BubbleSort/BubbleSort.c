#include <stdio.h>
#define N 6

int main(void) {
	int a[] = { 8,2,4,9,3,6 };
	int i, j, tmp;

	for (i = 0; i < N - 1; i++) {
		for (j = 0; j < N - i - 1; j++) {
			if (a[j] > a[j + 1]) {
				tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
	}
	for(i=0;i<N;i++)
		printf("%d", a[i]);
	getch();
	return 0;
}