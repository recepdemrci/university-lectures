#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FRND 25
#define LINE_SIZE 255

FILE *fi;

// Istenilen özellikteki verileri tutan yapi
typedef struct {
	int IdNumber;
	char nameSurname[50];
	int friendsId[MAX_FRND];

	struct User *left;
	struct User *right;
}User;

//--------------------- Fonksiyonların prototipleri -----------------------------------------------------------------------------------

int CharToInt(char[], int);							// String'i integer'a çevirir.(Bu fonksiyon önceki ödevden aynen alınıp kullanıldığı için açıklaması yapılmadı.)
void getInfo(User *	);								// Gelen kulanıcının nameSurname ve friensId kısmını dosyadan okuyarak doldurur.
User *insertNewUser(User* , int);					// Kimlik Numarası verilen kullanıcının BT'deki doğru yerini bulur ve ekler.
User *createBT();									// input.txt dosyasındaki kullanıcılardan Binary Tree oluşturur.
User *deleteUser(User *, int);						// Verilen ağaçtaki Verilen kimlik numarasını bularak ağaçtan siler.
User *searchMin(User *);							// Verilen ağacın en küçük kimlik numarasına sahip olan kullanıcıyı bulur. 
void change(User *, User *);						// Verilen ikinci düğümdeki sadece bazı değerleri ilk düğüme yazar.
int size(User *);									// Ağaçtaki toplam kullanıcı sayısını bulur.
void contains(User *, int);							// Fonksiyona gönderilen kimlik numarasındaki kişi ağaçta varsa onu bulup ismini yazdırır.
void contains_v2(User *, int);						// contains fonksiyonundan farklı olarak her karşılaştırdığı kullanıcının kimlik numarasını yazdırmıyor.
void friends(User *, int);							// Verilen kimlik numarası ağaçta varsa bulup ağaçta olan arkadaşlarının ismilerini yazdırır.
void printInOrder(User *);							// Verilen ağacı (left - node - right) sırasını takip ederek küçükten büyüğe yazdırır.
void printNext(User *, int);						// 
void printGreater(User *, int);						// Verilen kimlik numarasından daha büyük olanları küçükten büyüğe yazdırır.		

//-------------------------------------------------------------------------------------------------------------------------------------

int main(void) {
	
	User *userBT = NULL;
	int in_1 = -1;
	int in_2 = -1;

	while (in_1 != 0) {
		printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		printf("\n< Press 1 >  for createBinaryTree(insert All User)");
		printf("\n< Press 2 >  for deleteUser");
		printf("\n< Press 3 >  for contains");
		printf("\n< Press 4 >  for friends");
		printf("\n< Press 5 >  for size");
		printf("\n< Press 6 >  for printNext");
		printf("\n< Press 7 >  for printInOrder");
		printf("\n< Press 8 >  for printGrater");
		printf("\n< Press 0 >  for exit\n>");
		scanf("%d", &in_1);
		
		switch (in_1) {
			
		case 0: 
			printf("Exitting...");
			break;
	
		case 1:
			userBT = createBT();
			printf(">>Binary Tree is created.");
			break;
			
		case 2:
			printf(">>Please ENTER the user id number(0 - 10000): ");
			scanf("%d", &in_2);
			
			if (in_2 <= 0 || in_2 >= 10000) {
				printf("< Invalid id number! >");
			}
			else {
				deleteUser(userBT, in_2);
				printf("< Operation is completed. >");
			}
			break;
			
		case 3:
			printf(">>Please ENTER the user id number(0 - 10000): ");
			scanf("%d", &in_2);

			if (in_2 <= 0 || in_2 >= 10000) {
				printf("< Invalid id number! >");
			}
			else {
				contains(userBT, in_2);
			}
			break;

		case 4:
			printf(">>Please ENTER the user id number(0 - 10000): ");
			scanf("%d", &in_2);
			
			if (in_2 <= 0 || in_2 >= 10000) {
				printf("< Invalid id number! >");
			}
			else {
				friends(userBT, in_2);
			}
			break;

		case 5:
			printf("< size: %d >", size(userBT));
			break;

		case 6:
			printf(">>Please ENTER the user id number(0 - 10000): ");
			scanf("%d", &in_2);

			if (in_2 <= 0 || in_2 >= 10000) {
				printf("< Invalid id number! >");
			}
			else {
				printNext(userBT, in_2);
			}
			break;

		case 7:
			printInOrder(userBT);
			break;

		case 8:
			printf(">>Please ENTER the user id number(0 - 10000): ");
			scanf("%d", &in_2);

			if (in_2 <= 0 || in_2 >= 10000) {
				printf("< Invalid id number! >");
			}
			else {
				printGreater(userBT, in_2);
			}
			break;

		default:
			printf("< Invalid number! Pelase try again. >");
			break;
		}
	}
	
	return 0;
}

//--------------------- Fonksiyonlar ---------------------------------------------------------------------------------------------------
int CharToInt(char str[], int strSize) {
	int i;
	int digit, number = 0;

	for (i = 0; i < strSize; i++) {
		digit = str[strSize - i - 1] - 48;
		number += (digit * pow(10, i));
	}
	return number;
}

User *createBT() {														
																		// ----------------------------------------------------------------------------
	User *root = NULL;													// Binary Tree' nin kök düğümünü gösterecek pointer oluşturuldu.
																		// fi pointere global olarak yukarıda tanımlanmıştır.
	fi = fopen("input.txt", "r");										// input.txt dosyası okuma işlemi yapılacak şekilde açıldı.
	if ( fi == NULL ) { exit(-1); }										// Eğer dosya açılamadıysa program sonlandırılır. 

	char str[6];														// Kullanıcının kimlik numarasını geçici olarak tutması için str karakter
	int i;																// dizini oluşturuldu.

	while (!feof(fi)) {													//  ----
																		//	Kullanıcı Kimlik Numarasnı elde etmek için
		i = 0;															// 	İlk virgül gelene kadar dosyadan karakter karakter			
		str[i] = fgetc(fi);												//  okuma yapılır ve str isimli stringde tutulur.
																		// 	Sonra str CharToInt fonksiyonuyla integer'a çevrilir.
		if (str[i] >= '0' && str[i] <= '9') {							//  !!!!! BU KONTROLÜ YAPMAMIN SEBEBİ DOSYA SONUNA GELİNDİĞİNİ ANLAMAK. SATIR BİTİNCE İLK GELEN KARAKTER RAKAMDAN BAŞKA BİŞEY OLAMAZ.
			while (str[i] != ',') {										// 
				i++;													//	Kullanıcının geri kalan bilgilerini okumak ve ağaca eklemek için 		
				str[i] = fgetc(fi);										//	BT'nin kökü ve kimlik numarası insertNewUser fonksiyonuna gönderilir.	
			}															//
			root = insertNewUser(root, CharToInt(str, i));				//  Yukarıdaki işlemler dosya sonuna kadar herbir satır(kullanıcı) için			
		}																//  tekrar eder.
	}																	//  ----

	fclose(fi);															//  Dosya kapatıldı.
	return root;														//  BT' nin kökü fonksiyondan döndürüldü.
}																		// ----------------------------------------------------------------------------

User *insertNewUser(User *root, int IdNumber) {							// ----------------------------------------------------------------------------      
																		//  Özyinelemeli olarak çalışan bu fonksiyon gelen kimlik numarasına
																		//  bakarak kullanıcının BT'deki yerini bulur. 
	
	if (root == NULL) {													//	Kullanıcının BT'deki yeri bulununca: 					
		root = (User *)malloc(sizeof(User));							//	O düğümde yer oluşturulur ve kullanıcının bilgileri bu düğüme kaydedilir:			
		root->IdNumber = IdNumber;										//	  >Kimlik bilgisi yazılır.
		getInfo(root);													//	  >İsim ve arkadaş bilgisi yardımcı bir fonksiyon ile dosyadan okunup
		root->left = NULL;												//	   düğümün ilgili yerlerine yazılır.
		root->right = NULL;												//										
																		//
		return root;													//	Oluşturulan düğüm döndürülür.  
	}
	else if (IdNumber < root->IdNumber) {								//  Eğer kimlik numarası bulunduğumuz düğümden küçükse:
		root->left = insertNewUser(root->left, IdNumber);				//	Düğümün sol tarafı aynı fonksiyona gönderilir. 	
	}																						
	else {																//  Eğer kimlik numarası bulunduğumuz düğümden büyükse:		
		root->right = insertNewUser(root->right, IdNumber);				//	Düğümün sol tarafı aynı fonksiyona gönderilir.
	}
	return root;		
}																		// ----------------------------------------------------------------------------      

void getInfo(User *newUser) {																	
																		// -----------------------------------------------------------------------------
	int i, j, k;														// DEĞİŞKENLER:
	char buff[LINE_SIZE];												// Dosyadan okunan bir satır buff isimli değişkende tutulur.
	char str[6];														// str, string tipinde geçici olarak friendsId 'yi tutar.
																		// -----------------------------------------------------------------------------																							
																		
																		// -----------------------------------------------------------------------------
	fgets(buff, LINE_SIZE, fi);											// Dosyada fi'nin kaldığı yerden(İsim kısmının başından) 
	i = 0;																// bir satır okunup buff'a atılır.
																		// -----------------------------------------------------------------------------
	while (buff[i] != ','  &&  buff[i] != 10) {							// (nameSurname)
		newUser->nameSurname[i] = buff[i];								// Virgül veya satır sonu karakteri gelene kadar buff'daki her bir karakter 
		i++;															// kullanıcının isim kısmına yazılır ve indis arttırılır.
	}																	//
	newUser->nameSurname[i] = 0;										// Bu işlem bitince isim kısmının sonuna 0 konulur.(Stringin sonu olduğunu belirtmek için.) 							
	
	j = 0;																// -----------------------------------------------------------------------------
	while (buff[i] != 10) {												// (friensId[])--------	
		i++;															// Satır sonu gelmemişse indis arttırılarak friendsId okunma işlemi başlar:
		k = 0;															// 	 	
																		//
		while (buff[i] != '-' && buff[i] != 10 ) {						// Bir sonraki friendsId'ye geçiş gelene kadar veya satır sonu olana kadar	
			str[k] = buff[i];											// str geçici karakter dizinine Id yazılır.	
			k++;														//	
			i++;														//
		}																//			
		newUser->friendsId[j] = CharToInt(str, k);						// str'deki karakter tipindeki Id, integer'a çevirilerek kullanıcının friendsId
		j++;															// kısmına yazılır.(Tüm arkadaşlar için aynı işlem tekrar eder)
	}																	// Bu işlem bitince friendsId dizisinin sonuna geldiği anlaşılsın diye dizi sonuna  
	newUser->friendsId[j] = 0;											// 0 koyulur.
}																		// -----------------------------------------------------------------------------


User *deleteUser(User *root, int IdNumber) {

	User *tmp;

	if (root == NULL) {
		return root;
	}

	if (IdNumber < root->IdNumber) {									//-----------------------------------------------------------
		root->left = deleteUser(root->left, IdNumber);					//	Bu kısımda silinecek kullanıcı 
	}																	//	arama işlemi yapılır.
	else if (IdNumber > root->IdNumber) {								//	
		root->right = deleteUser(root->right, IdNumber);				//-----------------------------------------------------------
	}
	else {																//  Kullanıcı bulunduktan sonra,
																		// ----------------------------------------------------------
		if (root->left == NULL) {										//	
			tmp = root->right;											//	Eğer kullanıcının hiç çocuğu yoksa
			free(root);													//	veya tek çocuğu varsa direk 
			return tmp;													//	kullanıcı silinir.
		}																//  Ve fonksiyondan çocuğu(çocuğu yoksa NULL) 
		else if (root->right == NULL) {									//	dönülür.
			tmp = root->left;											//		
			free(root);													//
			return tmp;													//-----------------------------------------------------------
		}
		else {															//-----------------------------------------------------------
			tmp = searchMin(root->right);								// Eğer kullanıcının iki çocuğu varsa;
			change(root, tmp);											// Silinecek kullanıcının sağ tarafındaki ağatan en küçük 
			root->right = deleteUser(root->right, tmp->IdNumber);		// kimlik numarasına sahip olan kullanıcı bulunur.
																		// Ve bilgileri silinecek olan kullanıcının adresine yazılır.
		}																// Son olarak o kullanıcıda ağaçtan silinir.
																		//------------------------------------------------------------
	}
	return root;
}

User *searchMin(User *root) {
																		//------------------------------------------------------------
	User *current = root;												// Kökü kaybetmeden ağaçta dolaşmak için current tanımlandı.				
																		//------------------------------------------------------------	
	while (current->left) {												// Null gelene kadar ağaçta hep sola gidilir.
		current = current->left;										// Ve en küçük eleman bulunmuş olur.
	}																	//------------------------------------------------------------	
	return current;
}

void change(User *user1, User *user2) {
	int i = 0;
																		//------------------------------------------------------------	
	user1->IdNumber = user2->IdNumber;									// Gelen 2. kullanıcının bilgileri 1. kullanıcının adresine
	strcpy(user1->nameSurname, user2->nameSurname);						// aktarılıyor.
	while (user2->friendsId[i] != 0) {									// Arkadaşlarıda tek tek aktarılır.(dizi sonuna gelindiğini 
		user1->friendsId[i] = user2->friendsId[i];						// anlamak için dizinin sonunda 0 değeri vardır)					
		i++;															//------------------------------------------------------------	
	}
	user1->friendsId[i] = user2->friendsId[i];
}


int size(User *root) {													//------------------------------------------------------------
																		// Her düğüm kendinin solundaki ve sağındaki ağacın boyutunu
	if (root == NULL) {													// toplar ve 1 ekler(kendisini). 			
		return 0;														// Yaprakların hiç çocuğu olmadığı için yaprakların solundan 
	}																	// ve sağından 0 gelir.  
	else {																// Yukarı doğru boyut artarak gider.
		return ( size(root->left) + size(root->right) + 1);				//
	}																	//------------------------------------------------------------
}


void contains(User *root, int IdNumber) {												
																		//-------------------------------------------------------------
	if (root == NULL) {													// Özyinelemeli olarak arama yapıp karşılaştıran bu fonksiyonda
		printf("\n< The User is not Found! >");
		return;															// verilen kimlik numarasına sahip kullanıcıyı bulamazsa 
	}																	// fonksiyondan çıkar.
																		//-------------------------------------------------------------
	
	if (root->IdNumber > IdNumber) {									//-------------------------------------------------------------
		printf("\n< ...Comparing: %d >", root->IdNumber);				// Eğer verilen kimlik numarası o düğümdeki kimlik numarasından
		contains(root->left, IdNumber);									// küçük ise ağacın soluna doğru arama yapmaya devam eder,					
	}																	// büyük ise ağacın sağına doğru arama yapmaya devam eder.
	else if (root->IdNumber < IdNumber) {								//
		printf("\n< ...Comparing: %d >", root->IdNumber);				// Her karşılaştırılan kullanıcının kimlik numarasını da 
		contains(root->right, IdNumber);								// yazdırır.		
	}																	//-------------------------------------------------------------

	else {																//-------------------------------------------------------------
																		// Karşılaştırılan kullanıcının kimlik numarasından büyük veya
		printf("\n< The User is found: %s >", root->nameSurname);		// küçük değilse aranılan kullanıcı bulunmuş demektir. Ve yazdırılır.
	}																	//-------------------------------------------------------------	

}
void contains_v2(User *root, int IdNumber) {
																		
	if (root == NULL) {													
		return;															
	}																																
	if (root->IdNumber > IdNumber) {									
		contains_v2(root->left, IdNumber);														
	}																	
	else if (root->IdNumber < IdNumber) {							
		contains_v2(root->right, IdNumber);									
	}																	
	else {																															
		printf("\n< %s >", root->nameSurname);
	}																	

}


void friends(User *root, int IdNumber) {									//----------------------------------------------------------------------------
	User *current = root;													// Gerçek kökü kaybetmemek için başka bir değişken üzerinden ağaçta gezilir.			
																			//----------------------------------------------------------------------------

	while (current != NULL && current->IdNumber != IdNumber) {				//----------------------------------------------------------------------------		
																			// Eğer bulunduğum düğüm istenilen kullanıcı değilse ve NULL değilse  
		if (IdNumber < current->IdNumber) {									// (ağacın sonuna gelinmediyse) kimlik numarasının küçük ve büyük olmasına bağlı
			current = current->left;										// olarak ağaçta sağa veya sola giderek arama yaparım.
		}																	//
		else {																//
			current = current->right;										//
		}																	//----------------------------------------------------------------------------	
	}																	

	if (current != NULL) {													//----------------------------------------------------------------------------			
		printf("< %s 'in agacta olan arkadaslari: >", current->nameSurname);// Eğer current, NULL değilse yukarıdaki döngüden istenilen kullanıcı bulunarak
		int i = 0;															// çıkılmıştır.
		while (current->friendsId[i] != 0) {								// Arkadaşlarının kimlik numarasının  bulunduğu dizinin	sonuna gelene kadar	
			contains_v2(root, current->friendsId[i]);						// her arkadaş ağaçta aranır. Ağaçta yer alıyorsa ismi yazdırılır. 
			i++;															// 
		}																	// Eğer arkadaşı yoksa ekrana YOK yazdırılır.(Arkadaşı var ama ağaçta 
		if (i == 0) { printf("\n< YOK >"); }								// bulunmuyorsa herhangi birşey yazılmaz).
	}																		//----------------------------------------------------------------------------
}


void printInOrder(User *root) {
	
	if (root == NULL) { 
		return; 
	}
																			
	printInOrder(root->left);											
	printf("\n%d", root->IdNumber);										 	
	printf("\t%s", root->nameSurname);									
	printInOrder(root->right);											
}																		

void printNext(User *root, int IdNumber) {
	
	if (root == NULL) {
		return; 
	}

	if (IdNumber < root->IdNumber) {
		printNext(root->left, IdNumber);
	}
	else if (IdNumber > root->IdNumber) {
		printNext(root->right, IdNumber);
	}
	else {
		printInOrder(root);
	}
}

void printGreater(User *root, int IdNumber) {							
	
	if (root == NULL) { 
		return;
	}																		// Verilen kimlik numarasını bulmak için ağaçta sıra ile gezilir.
																			// --------------------------------------------------------------------------------
	if (IdNumber < root->IdNumber) {										// 
		printGreater(root->left, IdNumber);									// Kimlik numarası bulunduğum düğümdeki kimlik numarasından küçük ise ağaçta sola gidilir.
		printf("\n%d", root->IdNumber);										// Ve eğer ağaçta sola gidersem geri dnüşte bu düğüm ve düğümün sağ tarafı yazdırılır.
		printf("\t%s", root->nameSurname);									//
		printInOrder(root->right);											// --------------------------------------------------------------------------------																		
	}																		//
	else if (IdNumber > root->IdNumber) {									// Kimlik numarası bulunduğum düğümdeki kimlik numarasından büyük ise ağaçta sağa gidilir.
		printGreater(root->right, IdNumber);								// Ve eğer ağaçta sağa gidersem geri dönüşte bu düğüm ve düğümün sol tarafı yazdırılmaz.			
	}																		// Çünkü bu düğüm ve sol taraf verilen kimli numarasında küçüktür.
																			// --------------------------------------------------------------------------------
	else {																	//
		printInOrder(root->right);											// Eğer ağaçta fonksiyona verilen kimlik numarasına gelirsem, normal ınOrder sıra ile 							
	}																		// bulumduğum düğüm ve alt ağacı yazdırılır.		
}																			// --------------------------------------------------------------------------------	
