#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

#define N 15
#define M 29
#define R 3
#define MAX_FILE_NAME 50
#define MAX_WORD_SIZE 50
#define MAX_LINE_SIZE 255


struct Hash {
	unsigned long int key;
	char file_name[MAX_FILE_NAME];
};


//----------------------------FUNCTIONS--------------------------------------//
void create_hash_table();
void insert(char *, bool);
unsigned long int get_key(char *);

int hash_function(unsigned long int, int);
int h1(unsigned long int);
int h2(unsigned long int);

bool compare_files(char *, char *);
void write_to_samples(char *);
//---------------------------------------------------------------------------//

struct Hash *hash_table = NULL;

int main() {
	char file_name[MAX_FILE_NAME];
	int flag = 1;
	
	create_hash_table();
	printf("Hash Table has been created.\n");
	
	while(flag) {
		printf("Write the file name which you want to add: ");
		scanf("%s", file_name);
		insert(file_name, true);

		printf("\n> To exit press 0\n"
				 "> To continue press 1\n");
		printf(">>");
		scanf("%d", &flag);
	}

	
	getch();
}



void create_hash_table() {
	int i;
	char file_name[50];
	FILE *fi;


	hash_table = (struct Hash*)malloc(sizeof(struct Hash) * M);						// Define hash table, and allocate memory for it
	for (i = 0; i < M; i++) {														
		hash_table[i].key = 0;														// Assign all key = 0(NULL)
		strcpy(hash_table[i].file_name, "EMTY");									// Write 'EMTY' to all filename.
	}

	fi = fopen("samples.txt", "r");													// Control if file has been opened correctly.
	if (!fi) {																		// If there is a ERROR exit(-1)
		printf("> ERROR Opening samples.txt (for read)\n");
		exit(-1);
	}

	while (!feof(fi)) {
		fscanf(fi, "%s", file_name);												// Read file's name from samples.txt, 
		insert(file_name, false);													// and add it to the hash_table.
	}
	fclose(fi);
}


void insert(char *file_name, bool write_samples) {									// Take filename and write_samples as a parameter.
	int i;																			// If write_samaples == false then file is already in samples.txt
	int hash_index;
	char saved_file_name[MAX_FILE_NAME];
	bool flag = false;
	struct Hash newFile;															
	 																				
	newFile.key = get_key(file_name);												// Newfile structure is created,
	strcpy(newFile.file_name, file_name);											// and aranged their value with it's filename and key

	i = 0;
	while (!flag) {
		
		hash_index = hash_function(newFile.key, i);
																					// YOU SHOULD ADD HERE CONTROL IF TABLE IS FULL::::::::
		if (hash_table[hash_index].key == NULL) {			                		// if the hash adress is emty, write NewFile that adress
			hash_table[hash_index] = newFile;
			if (write_samples == true) { write_to_samples(file_name); }				// If i read filename from samples, DO NOT write it to the samples.txt again
			flag = true;
		}
		else {
			strcpy(saved_file_name, hash_table[hash_index].file_name);              // if the hash adress is NOT emty,               
			if (compare_files(saved_file_name, file_name)) {						// Control if thats file are same
				flag = true;
				printf("> File is already in the database\n");
			}
			else {
				i++;																// if the collusion files are not same, 
			}																		// increase i and calculate hash adress again
		}
	}
}


unsigned long int get_key(char *file_name) {
	int i;
	int number_of_char;
	double n;
	unsigned long int key;
	char buff[MAX_WORD_SIZE];														// POINTER OLUR MU BAKMALISIN
	bool flag;
	FILE *fi;

	fi = fopen(file_name, "r");														// Control if file has been opened correctly
	if (!fi) {																		// If there is a ERROR exit -3
		printf("> ERROR Opening %s \n", file_name);
		exit(-3);
	}

	key = 0;
	while (!feof(fi)) {

		i = 0;
		flag = true;
		while (flag) {
			buff[i] = fgetc(fi);													// Read one character for each time
			if ((buff[i] == ' ') || (buff[i] == '\n') || feof(fi)) {				// Control if word has been read
				flag = false;
			}
			else {
				i++;																// 'i' represents number of char in that word
			}
		}
		number_of_char = i;

		for (i = 0; i < number_of_char; i++) {
			n = number_of_char - i - 1;
			key += (buff[i] * pow(R, n));											// key = str[0] * Rn-1 + str[1] * Rn-2 + ... + str[n-1]
		}
	}
	fclose(fi);

	return key	;
}


int hash_function(unsigned long int key, int i) {
	return ( (h1(key) + i * h2(key)) % M );
}

int h1(unsigned long int key) {
	return (key % M);
}

int h2(unsigned long int key) {
	return (1 + (key % (M - 1)));
}


bool compare_files(char *file_name_1, char *file_name_2) {							// if two files are equal return true, otherwise return false
	FILE *fi_1, *fi_2;
	static char buff_1[MAX_LINE_SIZE], buff_2[MAX_LINE_SIZE];

	fi_1 = fopen(file_name_1, "r");	if (!fi_1) { printf("> ERROR opening File1\n");	exit(-4); }
	fi_2 = fopen(file_name_2, "r");	if (!fi_2) { printf("> ERROR opening File2\n"); fclose(fi_1); exit(-5); }

	fseek(fi_1, 0, SEEK_END);														
	fseek(fi_2, 0, SEEK_END);														// Set file pointers to end of the file, 
	if (ftell(fi_1) != ftell(fi_2)) {												// Compare the file size if its are different.
		return false;
	}

	fseek(fi_1, 0, SEEK_SET);														
	fseek(fi_2, 0, SEEK_SET);														// Set file pointers to start of the file,
	while ( !feof(fi_1) && (strcmp(buff_1, buff_2)==0) ) {							// Compare each lines of the files are same.
		fscanf(fi_1, "%s", buff_1);
		fscanf(fi_2, "%s", buff_2);
	}

	if (feof(fi_1)) {																// If fi_1 is on the end of the file, files are same.
		fclose(fi_1);																// Otherwise files are not same.
		fclose(fi_2);
		return true;																
	}																				
	else{ 
		fclose(fi_1);
		fclose(fi_2);
		return false; 
	}
}


void write_to_samples(char *file_name) {
	FILE *fi;

	fi = fopen("samples.txt", "a");													// Control if samples file has been opened
	if (!fi) {																		
		printf("> ERROR Opening samples.txt (for write) \n");
		exit(-2);
	}
	fputc('\n', fi);
	fprintf(fi, file_name, "%s");
	fclose(fi);
}