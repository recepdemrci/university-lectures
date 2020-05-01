#include<stdio.h>
#include<stdlib.h>


int *createShiftTable(int m, char *P, int alphSize) {				// alpSize is the number of symbol in alphabeth

	int i,adr;														// adr is index of symbol which is in the path[i]
	int *shiftTable=(int*)malloc(sizeof(int)*alphSize);

	for (i = 0; i < alphSize; i++) {
		shiftTable[i] = m;
	}
	for (i = 0; i < (m-1) ; i++) {
		adr = P[i] - 'A';
		shiftTable[adr] = m - i - 1;
	}

	return shiftTable;
}

int Horspool(int n, char *T, int m, char *P, int alphSize) {
	int i, k, adr;								
	int *shiftTable;

	shiftTable = createShiftTable(m, P, alphSize);

	i = m - 1;
	while (i < n) {
		
		k = 0;
		while ((k < m) && (P[m - k - 1] == T[i - k])) {
			k++;
		}
		
		if (k == m) {
			return i - m + 1;
		}
		else {
			adr = T[i] - 'A';										// shiftTable[adr] shows us that how many numbers we will shift
			i = i + shiftTable[adr];
		}
	}
	return -1;
}

void main(void) {

	int found, n = 26, m = 6, alphSize = 31;
	char T[27] = "JIM_SAW_ME_IN_A_BARBERSHOP";						// text
	char P[7] = "BARBER";											// pattern
	

	found = Horspool(n, T, m, P, alphSize);

	if (found != -1) {
		printf("%d", found);
	}
	
		getch();
}