#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define file_name "input-mpaa.txt"
#define MAX_LINE_SIZE 65536
#define MAX_MOVIE_NAME 128			// Movie's name size
#define MAX_ACTOR_NAME 128			// Actor's name size
#define MAX_ALL_MOVIE 16384			// Total number of movies(max)	
#define MAX_ALL_ACTOR 262144		// Total number of actor(max)
#define MAX_MOVIE 256				// Number of movies in one person
#define MAX_ACTOR 256				// Number of person in one movie

#define MAX_QUEUE_SIZE 262144

#define M 262144
#define R 3



/*------------------------------Structures & Lists-------------------------------*/
struct MOVIE {
	char movie_name[MAX_MOVIE_NAME];
	int number_of_actors;
	struct PERSON *actors[MAX_ACTOR];	// Actors who play in that movie												
};

struct PERSON {
	char person_name[MAX_ACTOR_NAME];
	int number_of_movies;
	struct MOVIE *movies[MAX_MOVIE];	// Movies which the actor played												
};

struct MOVIE *movie_list;				// List of the all movies
struct PERSON *actor_list;				// List of the all actor


/*------------------------Queue Variables-----------------------------------------*/
struct PERSON Queue[MAX_QUEUE_SIZE];   // Queue will store Persons
unsigned int item_count = 0;		   // Counter
int f = 0, r = -1;					   // r: points to end of the queue(when add new person)
									   // f: points to beginning of the queue(when remove a person)




/*-----------------------FUNCTION PROTOTYPE---------------------------------------*/
void initialize();																	// Read movies and actors from the file and assign to lists(graph)
void manage_line(char *);															// Take one line as a parameter, and split them based on '/'
int add_movie(char *);																// Create movie node and add this movie to the movie_list
void add_actor(char *, int);														// Create actor node and add this actor to the actor_list

void bfs(char *, char *);															// While doing breath-first search, add every parent actor and movie into the path array 
void get_result(char *[MAX_ALL_ACTOR], char *[MAX_MOVIE_NAME], char *, char *);		// Get result: apply bactrack method by using path arrays


long int find_actor(char *);														// Find index of actor from hashTable of actors (if not found, return negative value)
unsigned long int get_key(char *);													// Calculate key value by using actor names ASCII values
long int hash_function(unsigned long int, int);										// Hash functions (Double hashing is used in case of collusion)
long int h1(unsigned long int);
long int h2(unsigned long int);

void initialize_queue();															// Initialize(Reset) Queue indexes in every bfs function is called
bool is_emty();																		// Control if queue is emty
bool is_full();																		// Control if queue is full
void enqueue(struct PERSON);														// Add new PERSON end of the queue
struct PERSON dequeue();															// Remove a PERSON beginning of the queue
/*--------------------------------------------------------------------------------*/




void main() {
	char choice;
	char dummy;
	char destination_name[MAX_ACTOR_NAME];
	char source_name[MAX_ACTOR_NAME];


	initialize();																   // Initialize the program and create the graph
	while (1) {
		printf("\n-----------LET THE GAME BEGIN-----------\n");
		printf("Press 1: to find Kevin Bacon number\n");
		printf("Press 2: to find 2 person's distance\n");
		printf("Press 0: to exit\n");
		printf("Make your choice or die: ");										// INPUT FORMAT : Surname, Name
																					
		scanf("%c", &choice);
		scanf("%c", &dummy);
		
		switch (choice) {
			case '1':
				printf("Enter person name who you want to find(Surname, Name): ");
				gets(destination_name);
				strcpy(source_name, "Bacon, Kevin");
				bfs(source_name, destination_name);
				break;
			case '2':
				printf("Enter first person name(Surname, Name): ");
				gets(source_name);
				printf("Enter second person name(Surname, Name): ");
				gets(destination_name);
				bfs(source_name, destination_name);
				break;
			case '0':
				free(movie_list);
				free(actor_list);
				exit(1);
			default:
				printf("You made a wrong choice\n");
		}
	}

	getch();
}


// Initialize the program
// Read movies and actors from the file and assign to lists
void initialize() {
	int i;
	FILE *fi;
	char buffer[MAX_LINE_SIZE];


	fi = fopen(file_name, "r");
	if (fi == NULL) { exit(-1); }													// Control if any error is occured while opening file

	movie_list = (struct MOVIE *)malloc(sizeof(struct MOVIE)*MAX_ALL_MOVIE);		// Allocate memory for Adjaceny lists
	if (movie_list == NULL) { exit(-2); }
	actor_list = (struct PERSON *)malloc(sizeof(struct PERSON)*MAX_ALL_ACTOR);
	if (actor_list == NULL) { exit(-3); }
	for (i = 0; i < MAX_ALL_ACTOR; i++) {
		strcpy(actor_list[i].person_name, "NULL");
	}


	while (!feof(fi)) {
		fgets(buffer, MAX_LINE_SIZE, fi);											// Read line by line from file
		manage_line(buffer);
	}	

	fclose(fi);
}


// Take one line as a parameter, and split them based on '/' 
void manage_line(char *line) {
	int name_size;
	bool flag = true;
	int movie_index;
	char buffer[MAX_LINE_SIZE];
	char *movie_name, *actor_name;													// Store actors and movies name temporarily

	strcpy(buffer, line);

	movie_name = strtok(buffer, "/");												// split the line based on "/"(slash) 
	movie_index = add_movie(movie_name);											// ADD movie into the graph
	
	while (flag) {
		actor_name = strtok(NULL, "/");
		if (actor_name == NULL) { 
			flag = false; 
		}
		else {
			name_size = strlen(actor_name);
			
			if (actor_name[name_size - 1] == '\n') {								// Remove end of the line character and add null character
				actor_name[name_size - 1] = NULL;
			}
			add_actor(actor_name, movie_index);										// ADD actor into the graph
		}
	}
}


// Create movie node and add this movie to the movie_list
// return count : for add actors into that movie by using these count variable as index
int add_movie(char *movie_name) {
	struct MOVIE the_movie;
	static int movie_count;															// Counter of the movies

	strcpy(the_movie.movie_name, movie_name);										// Assign movie name
	the_movie.number_of_actors = 0;													// Assign number of actors in the movie
	movie_list[movie_count] = the_movie;											// Add movie to the list 
	movie_count++;																	// Increase the counter of movies

	return (movie_count-1);
}


// Create actor node and add this actor to the actor_list
void add_actor(char *actor_name, int movie_index) {
	struct PERSON the_actor;
	long int actor_index, index;

	actor_index = find_actor(actor_name);										    // If actor does not exist, actor_index = (-1*real_index)
	if (actor_index >= 0) {

		index = actor_list[actor_index].number_of_movies;							// ADD the movie into the actor's movies
		actor_list[actor_index].movies[index] = &movie_list[movie_index];			//
		actor_list[actor_index].number_of_movies++;									//

		index = movie_list[movie_index].number_of_actors;							// ADD the actor into the movie's actors
		movie_list[movie_index].actors[index] = &actor_list[actor_index];			//
		movie_list[movie_index].number_of_actors++;									//

	}
	else {
		actor_index *= (-1);														// actor_index has negative index, change it positive

		strcpy(the_actor.person_name, actor_name);									// Assign actor name
		the_actor.movies[0] = &movie_list[movie_index];								// Assign actor's movies.
		the_actor.number_of_movies = 1;												// Assign number of movies, that is played by actor

		actor_list[actor_index] = the_actor;										// Write actor to the actor_list
	
		index = movie_list[movie_index].number_of_actors;								
		movie_list[movie_index].actors[index] = &actor_list[actor_index];			// Save the actor into the movies actor array
		movie_list[movie_index].number_of_actors++;									// Increase the number of actors who plays in the movie

	}
}



// Take actor names (default is Kevin Bacon),
// While doing breath-first search, add every parent actor and movie into the path array   
void bfs(char *source_name, char *destination_name ) {
	int i, j;																		
	bool flag = false;																// Flag controls if destination name is found
	int *marked_actor_list;															// Store the list of mark of actors

	int number_of_movies;															// Number of movies in the_actor
	int number_of_actors;															// Number of actors in the movie
	struct PERSON the_actor;														// In every dequeue operations, the_actor will store removed actor from the queue
	struct PERSON current_actor;													// current_actor is connected every actors to the_actor
	long int index;
	long int current_actor_index;

	char **path_actor;																// Store parent actor name in this array
	char **path_movies;																// Store parent movie in this array
	
	/*		Memory allocations for lists & control if any failaure		*/
	marked_actor_list = (int *)malloc(sizeof(int) * MAX_ALL_ACTOR);				
	if (marked_actor_list == NULL) { exit(-4); }
	
	path_actor = (char **)malloc(sizeof(char*) * MAX_ALL_ACTOR);
	for (i = 0; i < MAX_ALL_ACTOR; i++) {
		path_actor[i] = (char *)malloc(sizeof(char) * MAX_ACTOR_NAME);
	}
	if (path_actor == NULL) { exit(-5); }

	path_movies = (char **)malloc(sizeof(char*) * MAX_ALL_ACTOR);
	for (i = 0; i < MAX_ALL_ACTOR; i++) {
		path_movies[i] = (char *)malloc(sizeof(char) * MAX_MOVIE_NAME);
	}
	if (path_movies == NULL) { exit(-6); }
	/*-------------------------------------------------------------------*/


	for (i = 0; i < MAX_ALL_ACTOR; i++) {											// Mark all actors(Vertex) as "unvisited"
		marked_actor_list[i] = -1;
	}

	index = find_actor(source_name);												// Find and start from source_name(default is Kevin Bacon)
	if (index < 0) { printf("> Wrong input\n"); exit(-7); }
	
	marked_actor_list[index] = 0;													// Mark the_actor as 0(default kevin bacon)
	the_actor = actor_list[index];
	initialize_queue();																// Initialize(reset) the queue in every bfs() function called
	enqueue(the_actor);																// Add the actor into the queue
	
	while (!is_emty() && (!flag)) {

		the_actor = dequeue();
		number_of_movies = the_actor.number_of_movies;
		
		i = 0;
		while ((i < number_of_movies) && (!flag)) {
			number_of_actors = the_actor.movies[i]->number_of_actors;				// Look all the_actor's all movies, until find the destination 
			
			j = 0;
			while ((j < number_of_actors) && (!flag)) {								// Look all the actors in the movie, until find the destination
				
				current_actor = *the_actor.movies[i]->actors[j];
				current_actor_index = find_actor(current_actor.person_name);
				if (marked_actor_list[current_actor_index] == -1) {								// Control if current_actor is unvisited(-1)
					marked_actor_list[current_actor_index] = 0;									// Mark every adjaceny actor with count(that shows Kevin Bacon Number)
					enqueue(current_actor);														// Add the current_actor in the queue

					strcpy(path_actor[current_actor_index], the_actor.person_name);				// Add parent actor to the path
					strcpy(path_movies[current_actor_index], the_actor.movies[i]->movie_name);	// Add parent movie to the path
				}
				
				if (strcmp(current_actor.person_name, destination_name) == 0) {					// If the destination_name is found, then flag = true
					flag = true;
				}
				j++;
			}
			i++;
		}
	}

	if (!is_emty()) {
		get_result(path_actor, path_movies, source_name, destination_name);
	}
	else {
		printf("> Not found\n");
	}


	free(marked_actor_list);
	free(path_actor);
	free(path_movies);
}


// Get result: apply bactrack method by using path arrays
void get_result(char *path_actor[MAX_ALL_ACTOR], char *path_movies[MAX_MOVIE_NAME], char *source_name, char *destination_name) {
	bool flag = false;																// Flag controls if all path is written
	int count;																		// Kevin Bacon number (distance)
	long int source_index;															// Index of source name in the actor_list
	long int destination_index;														// Index of destination name in the actor_list	
	char *actor_name;																
	char *movie_name;


	
	source_index = find_actor(source_name);											
	destination_index = find_actor(destination_name);
	count = 0;
	while (!flag) {

		if (destination_index == source_index) {									// Control if all path is written
			flag = true;
		}
		else {																		// If all path is not written,		
			actor_name = path_actor[destination_index];								// Take the actor who is in the path (in order)
			movie_name = path_movies[destination_index];							// and take the movie name which is in the path also

			printf("> %s - %s   [%s]\n", destination_name, actor_name, movie_name);	// Print the path
		
			destination_name = actor_name;											// Update destination name as a current actor name
			destination_index = find_actor(destination_name);						// Update destinaton index as a current actor index
			
			count ++;
		}	
	}

	printf("> Distance: %d\n", count);

}


/*-----------------------------------QUEUE-----------------------------------*/
// Initialize(Reset) Queue indexes in every bfs function called
void initialize_queue() {
	f = 0;
	r = -1;
	item_count = 0;
}

// Control is queue is emty
bool is_emty(){
	if (item_count == 0) { return true; }
	return false;
}				

// Control if queue is full
bool is_full() {
	if (item_count == MAX_QUEUE_SIZE) { return true; }
	return false;
}

// Add new PERSON end of the queue
void enqueue(struct PERSON the_actor){
	if (!is_full()) {																// If Queue is full, do not add new person
		
		if (r == (MAX_QUEUE_SIZE - 1)) {											// When r points to the end of the queue,
			r = -1;																	// change it's position as points to the begining of the queue
		}
		r++;
																					
		Queue[r] = the_actor;														// Add new Person
		item_count++;																// increase item_counter
	}
}

// Remove a PERSON begining of the queue
struct PERSON dequeue(){
	struct PERSON the_actor;

	the_actor = Queue[f];															// Take a person from the queue
	f++;																			

	if (f == MAX_QUEUE_SIZE) {														// When f points to the end of the queue,
		f = 0;																		// change it's position as points to the beginin of the queue
	}

	item_count--;																	// Decrease the item_counter
	return the_actor;
}


/*-----------------------------------HASHING-----------------------------------*/

// Control if actor is already in the list
// If actor is in the list return index of the actor
// If actor is not in the list return index(Actor should be inserted in this index)
//										|--> if actor is new, index value will be negative
long int find_actor(char *actor_name) {
	int i;
	bool flag = false;															   // Flag controls if index of actor is found
	long int actor_index;														   // Addressindex) of actor in the hash table
	unsigned long int key;														   // Key value of actor

	i = 0;
	while (!flag ) {																// Flag will control if index is found
																					// And this loop repeat maximum actor_count time(number of actor in that list) 
		key = get_key(actor_name);													// Calculate key value
		actor_index = hash_function(key, i);										// Get address index 
		actor_index++;																// Increase index in order to handle 0 adress

		if ( strcmp(actor_list[actor_index].person_name, "NULL") == 0) {			// If the index is emty,
			actor_index *= (-1);													// Return it as negative(Because its new actor) 
			flag = true;
		}
		else if (strcmp(actor_list[actor_index].person_name, actor_name) == 0){		// If there is same actor in that index,
			flag = true;															// Return index directly
		}
		else {
			i++;																	// If index in not emty and it is not same with current actor,
		}																			// Increase i, and calculate new index
	}

	return actor_index;
}


// Calculate the key value based on actor name, and return it
unsigned long int get_key(char *actor_name) {
	int i;
	int number_of_char;																// Number of character in actor_name
	double n;
	unsigned long int key;

	key = 0;
	number_of_char = strlen(actor_name);
	for (i = 0; i < number_of_char; i++) {
		n = number_of_char - i - 1;
		key += (actor_name[i] * pow(R, n));											// key = str[0]*R^(n-1) + str[1]*R^(n-2) + ... + str[n-1]
	}

	return key;
}


// Hashing function: takes the key, and i value 
// (Double Hashing is used for collusion) 
long int hash_function(unsigned long int key, int i) {
	return ((h1(key) + i * h2(key)) % M);
}

long int h1(unsigned long int key) {
	return (key % M);
}

long int h2(unsigned long int key) {
	return (1 + (key % (M - 1)));
}