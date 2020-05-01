#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXSIZE 50

char **vertex;
int **matrix;
int size;

//--------------------------------------------Fonksiyon Prototipleri---------------------

bool isConnected(char *, char *);			// Verilen iki kelime arasinda tek harf donusumu varsa true doner.
void **createVertex();						// Dosyadan kelimeleri okuyarak bunlari vertex string dizisinde tutar.
void **createAdjacencyMatrix();				// Komsuluk matrisi oluturur.
int findIndex(char *);						// Verilen kelimenin vertex dizisindeki indisini bulur.
void BFS(char *, char *);					// Verilen ilk kelimeden ikinci kelime donusumunu bulur ve ekrana basar.
//---------------------------------------------------------------------------------------


//------------------------------------------------Queue fonksiyonlari--------------------
char Queue[MAXSIZE][6];
int f = 0, r = -1;
int itemCount = 0;

bool isEmty() {
	if (itemCount == 0) { return true; }
	return false;
}

bool isFull() {
	if (itemCount == MAXSIZE) { return true; }
	return false;
}

void enqueu(char *word) {
	if (!isFull()) {

		if (r == MAXSIZE - 1) {
			r = -1;
		}

		r++;
		strcpy(Queue[r], word);
		itemCount++;
	}
}

char *dequeue() {
	char word[6];

	strcpy(word, Queue[f]);
	f++;
	if (f == MAXSIZE) {
		f = 0;
	}
	
	itemCount--;
	return word;
}
//-----------------------------------------------------------------------------------------



int main() {
	char str1[6], str2[6];

	printf("Ilk kelimeyi giriniz(5 harf):");
	scanf("%s", str1);
	printf("Ikinci kelimeyi giriniz(5 harf):");
	scanf("%s", str2);

	createVertex();									
	createAdjacencyMatrix();

	BFS(str2, str1);

	
	/*
	for (int i = 0; i < size; i++) {
		printf("%s\n", vertex[i]);
		
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] == 1) {
				printf("%s\n", vertex[j]);
			}
		}
		printf("\n");

	}
	*/
	getch();
	
}



bool isConnected(char *word_1, char *word_2) {				
	int i, count = 0;														
	for (i = 0; i < 5; i++) {													//---------------------------------------------------------------
		if (word_1[i] != word_2[i]) {											// Iki kelimenin farkli harflerini count degiskeni ile sayar.
			count++;															// Ve bu sayi 1'i gecerse false doner.
		}																		//---------------------------------------------------------------
	}

	if (count == 1) {
		return true;
	}
	return false;
}


void **createVertex() {
	
	FILE *fi;
	int i;

	fi = fopen("kelime.txt", "r");												//---------------------------------------------------------------
	if (fi == NULL) {															// kelime.txt dosyasi acildi. Eger dosya acma islemi basarisiz
		printf("ERROR openning file");											// olursa program sonlandirilir.
		exit(-1);																//---------------------------------------------------------------
	}
																				//---------------------------------------------------------------
	vertex = (char **)malloc(sizeof(char*));									// Kelimeler vertex ismli string dizisinde tutulur. En basta 
	vertex[0] = (char *)malloc(6 * sizeof(char));								// bir kelimeyi tutacak sekilde yer acildi.
																				//
	i = 0;																		//
	while (!feof(fi)) {															// Ve dosyadan her bir kelimeyi okudugunda vertex dizisinin boyutu 
		fgets(vertex[i], 7, fi);												// buyutulur ve oraya kelime yerlestitilir.
		vertex[i][5] = '\0';													// 
		i++;																	//
																				// Dosyadan 7 karakter okunmasinin nedeni kelimelerin sonunda '\n'
		vertex = (char**)realloc(vertex, (i+1) * sizeof(char*));				// bulunmasidir. Ve okuduktan sonra son karakter '\0' yapilir.
		vertex[i] = (char *)malloc(6 * sizeof(char));							//
	}																			//---------------------------------------------------------------

	fclose(fi);

	size = i;
}																				

void **createAdjacencyMatrix() {
	int i,j;

	matrix = (int**)malloc(size * sizeof(int*));								//---------------------------------------------------------------
	for (i = 0; i < size; i++) {												// Komsuluk matrisi icin yer ayrildi. Ve bu matris olusturulurken 
		matrix[i] = (int*)malloc(size * sizeof(int));							// kelimeler arasinda baglanti var ise o goze 1 yazildi.
																				//---------------------------------------------------------------
		matrix[i][i] = 0;
		for (j = 0; j < size; j++) {
			if (isConnected(vertex[i], vertex[j])) {
				matrix[i][j] = 1;
			}
			else {
				matrix[i][j] = 0;
			}
		}
	}
}


int findIndex(char *word) {
	
	int i = 0;																	//---------------------------------------------------------------
	while (strcmp(vertex[i], word) != 0 && i < size) {							// Parametre olarak alinan kelime vertex'de hangi indiste yer
		i++;																	// aldigini bulur ve fonksiyondan doner. 
	}																			// Eger bulamazsa -1 doner.
																				//---------------------------------------------------------------
	if (i == size) {
		return -1;
	}
	return i;
}

void BFS(char *word1, char *word2) {
	int *marked = (int *)malloc(size * sizeof(int));							//--------------------------------------------------------------------------			
	int *parent = (int *)malloc(size * sizeof(int));							// Baktigim kelimelere tekrar bakmamak icin marked dizisi olusturuldu.
	int i, j, wordIndex, count;													// Ve bir kelimeye nerden gelindigini bilmek icin parent dizisi olusturuldu.
	char tmpWord[6];															// 
																				//
	for (i = 0; i < size; i++) {												// marked dizisinin tum elemanlari 0 yapilir.
		marked[i] = 0;															// parent dizisinin tüm elemanlari -1 yapilir.
		parent[i] = -1;															//--------------------------------------------------------------------------
	}																			
																				
	
	enqueu(word1);																//--------------------------------------------------------------------------
																				// Ilk kelime kuyruga yerlestirildi. Ve bu kelimenin indis numarasi bulunarak																
	wordIndex = findIndex(word1);												// isaretlendi.
	marked[wordIndex] = 1;														//--------------------------------------------------------------------------

	strcpy(tmpWord, word1);

	while (!isEmty() && strcmp(tmpWord, word2) != 0) {							//--------------------------------------------------------------------------
																				// Queue bosalana kadar veya aranan kelime eslesinceye kadar sasagidaki  		
		strcpy(tmpWord ,dequeue());												// islemler tekrar eder.
		if (strcmp(tmpWord, word2) != 0) {										// 
																				//  Kuyruktan bir kelime alinir.
			wordIndex = findIndex(tmpWord);										//  Kelimenin vertex'deki indis numarasi bulunur.
			for (j = 0; j < size; j++) {										//  Kelimenin komsulari matrix dizisinde aranir. Ve isaretlenmemis ise
				
				if(wordIndex == -1){
					printf("<< Connection NOT found. >>");
					exit(-2);
				}
																				// kuyruga yerlestirilir.		
				if (matrix[wordIndex][j] == 1) {								//  Kuyruga yerlestirilen kelimeler icin isaretleme yapilir.
					if (!marked[j]) {											//  Parenti yazilir.
						enqueu(vertex[j]);										//--------------------------------------------------------------------------
						strcpy(tmpWord, vertex[j]);
						marked[j] = 1;
						parent[j] = wordIndex;
					}
				}
			}		
		}
	}

	if (strcmp(tmpWord, word2) != 0) {											// Eger kelime bulunmus ise parent dizisi kullanilarak 
		printf("<< Connection NOT found. >>");									// kelime donusumu yazilir.
	}
	else {
		count = 1;
		printf("\n>>%s ", tmpWord);
		wordIndex = findIndex(tmpWord);
		while (parent[wordIndex] != -1) {
			wordIndex = parent[wordIndex];
			printf(">>%s ", vertex[wordIndex]);
			count++;
		}

		printf("\n%d adimda kelime donusumu yapildi.", count);
	}
}
