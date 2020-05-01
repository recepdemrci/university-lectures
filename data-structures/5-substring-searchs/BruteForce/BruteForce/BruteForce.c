#include<stdio.h>

int bruteForce(int n, char *T, int m, char *P) {
	
	int i, j;

	for (i = 0; i < (n - m); i++) {
		
		j = 0;
		while ((j < m) && (P[j] == T[i + j])) {
			j++;
		}
		if (j == m) {
			return i;			
		}
	}
	return -1;
}


void main(void) {
	
	int found, n = 26, m = 6;
	char T[27] = "JIM_SAW_ME_IN_A_BARBERSHOP";					// text
	char P[7] = "BARBER";										// pattern
	
	found = bruteForce(n, T, m, P);
	if (found != -1) {
		printf("%d", found);
	}

	getch();
}