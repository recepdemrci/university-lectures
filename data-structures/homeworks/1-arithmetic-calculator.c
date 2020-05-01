#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

#define MAXSIZE 50

//---------------------------------------------------------------------------------------------------------------------------------//

struct node {																		// Stack yapisini olusturmak icin linked list kullanildi. Ve bu sebeple struct olusturuldu									  						
	int data;													
	struct node *next;
};

struct node *numberStack, *symbolStack, *tmp;				 						// Kullanilacak Stack degiskenlerinin tanimlanmasi 
int size = 0;																		// ve boyutunun tanimlanmasi 



//------------FONKSIYONLARIN PROTOTIPLERI-----------------------------------------------------------------------------------------//

int CharToInt(char[] , int);														// Stringten integera cevirme fonksiyonu
void compute(char);																	// Gelen parametredeki isarete gore numberStackten iki sayi alarak islemi yapar ve sonucu numberStacke koyar
int priority(char, char);


void initial(struct node*);															//Linked Listenin baslangicini olusturan fonksiyon
struct node *addFront(struct node*, char);											//Linked Listenin baslangicina eleman ekleyen fonksiyon
struct node *deleteFront(struct node*);												//Linked Listenin baslangicindan eleman silen fonksiyon


struct node* push(struct node*, int);												//Stack'e eleman koyma islemini yapan fonksiyon
struct node* pop(struct node*, int*);												//Stack'ten eleman ceken fonksiyon
int peek(struct node*);																//Stack'teki son elemana bakma için fonksiyon
void displayChar(struct node*);														//Karakterden olusan bir Stack'in hepsini yazdiran fonksiyon
void displayInt(struct node*);														//Tam sayilardan olusan bir Stack'in hepsini yazdiran fonksiyon
void display();
bool isEmty(struct node*);															//Stack'n bos olup olmadigini kontrol eden fonksiyon	

//--------------------------------------------------------------------------------------------------------------------------------//


int main(void) {
	int i,k;
	char tmpSymbol;
	char str[MAXSIZE], tmpNumber[4];
	

	printf(">> Lutfen yapmak istediginiz islemi bosluk birakmadan giriniz: ");						
	scanf("%s",&str);
		
	initial(numberStack);																	// Stackler icin linked list baslangic degeri(NULL) olusturuldu
	initial(symbolStack);
	
	i=0;
	while(str[i]){																			//---------------------------------------------------------------		
	
		if((str[i] >= '0') && (str[i] <= '9')){												// Girilen islemdeki siradaki deger sayi ise o sayiyinin tamamini bulur 	
			k = 0;																			// Ve sayiyi int'e ceviren fonksiyona gönderir.
			while(str[i] >= '0'){
				tmpNumber[k] = str[i];
				k++;
				i++;
			}
			numberStack = push(numberStack, CharToInt(tmpNumber, k));						// Integera dönusturulen sayi Sayi Stackine atilir
			display();			
		} 
		
		if(str[i]){																			// Stringi tekrar kontrol eder(Bitmis olma durumuna karsi)
			switch(str[i]){																	//	Gelen karakteri 3 durumda inceleriz:
			
			case '(' : 																		// 1- parantez acma ise direk Stacke atilir
			
				symbolStack = push(symbolStack, str[i]);
				display();
				
				break;
				
			case ')' :																		// 2- parantez kapama ise Stackten parantez acma gelene kadar karakter çekilir 																							
				symbolStack = pop(symbolStack, &tmpSymbol);									// ve Gelen karaktere göre islem yapilir
				while(tmpSymbol != '(' ){
					compute(tmpSymbol);
					display();
					symbolStack = pop(symbolStack, &tmpSymbol);	
				}
				display();
				
				break;
				
			default :																		// 3- Herhangi bir islem isareti gelirse:
				
				if(isEmty(symbolStack)){														// a- Stack bos ise direk stacke atilir
					symbolStack = push(symbolStack, str[i]);	
					display();
				}
				else{																			// b- Gelen isaret ile Stackteki isaret arasinda öncelik karsilastirmasi yapilir
					tmpSymbol = peek(symbolStack);												//(Bu karsilastirma icin priority adinda fonksiyon olusturuldu)	
					if( (tmpSymbol == '(') || !priority(str[i], tmpSymbol) ){							// Stackten gelen düsük öncelikli ise veya parantez acma ise gelen islem Stacke atilir
						symbolStack = push(symbolStack, str[i]);
						display();
					}
					else{																				// Stackten gelen öncelikli veya esit öncelikli oldugu sürece Stackten sembol cekilerek islem yapilir.
						while(priority(str[i], tmpSymbol) && (!isEmty(symbolStack)) ){					// (Sembol Stackinin bosalma durumuda göz önünde bulundurulur.)
							symbolStack = pop(symbolStack, &tmpSymbol);
							compute(tmpSymbol);
							display();
							tmpSymbol = peek(symbolStack);	
						}
						symbolStack = push(symbolStack, str[i]);
						display();
					} 
				}
				
				break;
			}
			i++;	
		}		
	}
	
	while( !isEmty(symbolStack) ){															// Stringteki islem bittikten sonra Stackte islem kalmissa onlar yapilir
		symbolStack = pop(symbolStack, &tmpSymbol);
		compute(tmpSymbol);
		display();
	}
	
	printf("\nSonuc:\t");
	displayInt(numberStack);
	
	
	getch();
	return 0;
}


int CharToInt(char str[], int strSize){
	int i; 
	int digit, number = 0;

	for(i = 0; i < strSize; i++){
		digit = str[strSize - i - 1] - 48;
		number += (digit * pow(10, i));
	}
	return number;
}
void compute(char symbol){
	int number1, number2, result;
	
	numberStack = pop(numberStack, &number2);
	numberStack = pop(numberStack, &number1);
	switch(symbol){
		case '*' :
			result = (number1 * number2);
			break;
		case '/' :
			result = (number1 / number2);
			break;
		case '+' :
			result = (number1 + number2);
			break;
		case '-' :
			result = (number1 - number2);
			break;
	}
	numberStack = push(numberStack, result);
}
int priority(char x, char y){
	
	if( (y == '(') )															//Eger stackteki son eleman '(' ise priority fonksiyonu 0 döner.
		return 0;

	if( ((y == '+') || (y == '-'))  &&  ((x == '*')  || (x == '/')) ){			//Eger 	stackteki son eleman '+' veya '-' iken gelen sembol '*' veya '/' ise stackteki sembol düsük önceliklidir ve 0 döner.					
		return 0;
	}
	else
		return 1;																// Dger durumlarda 1 döner.
}



void initial(struct node *root) {
	root = NULL;
}
struct node *addFront(struct node* root, char value) {
	struct node *p;
	p = malloc(sizeof(struct node));
	p->next = root;
	p->data = value;

	return p;
}
struct node *deleteFront(struct node *root) {
	struct node *tmp;

	tmp = root;
	root = root->next;
	free(tmp);
	return root;
}



struct node *push(struct node* Stack, int value) {
	if (Stack == NULL) {
		Stack = (struct node*)malloc(sizeof(struct node));
		Stack->data = value;
		Stack->next = NULL;
	}
	else {
		Stack = addFront(Stack, value);
	}
	size++;
	return Stack;
}
struct node *pop(struct node* Stack, int *value) {

	if (Stack != NULL) {
		*value = Stack->data;
		Stack = deleteFront(Stack);
		size--;
		return Stack;
	}
	else {
		printf("> Stack UNDERFLOW \n");
		return NULL;

	}
}
int peek(struct node* Stack) {
	if (Stack != NULL) {
		return Stack->data;
	}
	//printf("> Stack is EMTY \n");
	return -2;
}
void displayChar(struct node* Stack) {
	tmp = Stack;

	if (tmp != NULL) {
		while (tmp != NULL) {
			printf("%c  ", tmp->data);
			tmp = tmp->next;
		}
		printf("\n");
	}
	else {
		printf("BOS\n");
	}
}
void displayInt(struct node* Stack) {
	tmp = Stack;

	if (tmp != NULL) {
		while (tmp != NULL) {
			printf("%d  ", tmp->data);
			tmp = tmp->next;
		}
		printf("\n");
	}
	else {
		printf("BOS\n");
	}
}
void display(){
	printf("\n");
	printf("Sayi Yigini: "); displayInt(numberStack);
	printf("Isaret Yigini:  "); displayChar(symbolStack);
	printf("---------------------------\n");
}
bool isEmty(struct node* Stack){
	if (Stack == NULL) {
		return true;
	}
	else {
		return false;
	}
}

