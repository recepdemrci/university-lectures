#include <stdio.h>

#define N 6

int main(void) {
	int i, j;
	int a[] = { 5,7,9,6,3,7 };
	static int c[N], s[N];

	for (i = 0; i < N - 1; i++) {
		for (j = i + 1; j < N; j++) {
			if (a[i] > a[j]) {
				c[i] += 1;
			}
			else {
				c[j] += 1;
			}
		}
	}

	for (i = 0; i < N; i++) {
		s[c[i]] = a[i];
	}
	for (i = 0; i < N; i++) {
		printf("%d", s[i]);
	}

	getch();
	return 0;
}