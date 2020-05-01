#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define POINTSIZE 100

typedef struct {					 // This struct will be store the distance and coordinates of closest point
	float distance;
	int *pointA;
	int *pointB;
}MIN;


// FUNCTION PROTOTYPES-----------------------------------------------------------------------------------

MIN Closest_Pair(int **, int);														// Find closest pair with recursion
MIN bruteForce(int **, int);							                            // Find minimum distance with brute force 

void sort(int **, int, char);							                            // Sort given points matrix based on x or y coordinates
float findDistance(int *, int *);						                            // Find distance between two points
MIN findMin(MIN, MIN);																// Find the minimum value of given parameters
int findPoints_aroundMiddle_andSort(int **, int, float, int **);					// Find the points around the median point

int readFile(char *, int **);

// ------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {

	int i;																			// index variables
	int n;																			// number of poitns 
	char *fileName;																	// file path name
	int **points;																	// Points Matrix which include x and y colloums
	MIN dMin;																		// stores distance between closest pair


	if (argc != 2) {																// Control if given argument is wrong
		printf("Usage: <%s> <filePath> ", argv[0]);
		exit(0);
	}


	points = (int **)malloc(sizeof(int*) * POINTSIZE);								// Memory allocation for Points matrix
	for (i = 0; i < POINTSIZE; i++) {
		points[i] = (int *)malloc(sizeof(int) * 2);
	}

	if (points == NULL) {															// Control if memory allocation failed
		printf("Memory Allocation Failed\n");
		exit(-1);
	}

	fileName = argv[1];
	if (fileName == NULL) {
		printf("File Not Found\n");
		exit(-5);
	}

	n = readFile(fileName, points);

	sort(points, n, 'x');															// Sort points based on x coordinate
	printf("Sorted Array of Points:\n");											//TEST
	for (i = 0; i < n; i++) {														//TEST
		printf("%d %d\n", points[i][0], points[i][1]);								//TEST
	}																				//TEST

	dMin = Closest_Pair(points, n);													// Find the distance between closest pair 

	printf("\n\nRESULT\n");
	printf("~~~~~~~~\n");
	printf("Minimum distance: %f\nClosest Pair: (%d %d) (%d %d)\n", dMin.distance, dMin.pointA[0], dMin.pointA[1], dMin.pointB[0], dMin.pointB[1]);

	getch();
	return(0);
}


MIN Closest_Pair(int **points, int n) {

	int i;																			// index variables
	int middleIndis;																// Stores  the index of median point
	int *middlePoint;																// Stores the value of median point
	MIN dL, dR, d, dMin;															// dL: minimum distance in left side of the points, dR: minimum distance in right side of the points 
																					// d: distance between closest pair in aroundPoints 
	int **aroundPoints;																// Stores the points around the median point
	int sizeofAroundPoints;

	if (n <= 3) {																	// if number of points are equal to or smaller than 3, find minimum distance by using bruteforce
		return bruteForce(points, n);
	}

	middleIndis = n / 2;
	middlePoint = points[middleIndis];

	dL = Closest_Pair(points, (middleIndis));										// Find distance between closest pair on left side of the points(recursively)
	dR = Closest_Pair(points + middleIndis, (n - middleIndis));						// Find distance between closest pair on right side of the points(recursively)
	dMin = findMin(dL, dR);															// Choose minimum distance between right and left points 

	printf("\n-----------------------------------\n");								//TEST
	printf("dLeft = %f\n", dL.distance);											//TEST
	printf("dRight = %f\n", dR.distance);											//TEST

	aroundPoints = (int **)malloc(sizeof(int *) * n);								// Memory allocation for aroundPointsMatrix
	for (i = 0; i < n; i++) {
		aroundPoints[i] = (int *)malloc(sizeof(int) * 2);
	}
	if (aroundPoints == NULL) {														// Control if memory allocation failed
		printf("Memory Allocation Failed\n");
		exit(-2);
	}

	sizeofAroundPoints = findPoints_aroundMiddle_andSort(points, n, dMin.distance, aroundPoints);	// Find points arround the median point and sort them based on y coordinate
	d = bruteForce(aroundPoints, sizeofAroundPoints);												// Find minimum distance in aroundPoints by using bruteforce


	if (d.distance < dMin.distance) {																// if d is smaller than dMin, update the dMin = d;  
		dMin = d;
	}

	printf("minimum distance around middle = %f\n", d.distance);						//TEST
	printf("dMin = %f\n", dMin.distance);											//TEST

	return dMin;
}

void sort(int **points, int n, char coordinate) {									// Take points matrix and number of points as parameters
																					// And sort it based on coordinates

	int i, j;																		// index variables
	int key_x, key_y;																// store the x,y of point while we are searching for the right place for it 

	if (coordinate == 'x') {

		for (j = 1; j < n; j++) {													// Look all the points and find right place for it
			key_x = points[j][0];
			key_y = points[j][1];
			i = j - 1;

			while (i >= 0 && key_x < points[i][0]) {								// if x value of the point is smaller then previous,
				points[i + 1][0] = points[i][0];									// swap the points(both x and y values) and compare with the other points
				points[i + 1][1] = points[i][1];									// Repeat these steps until x value of the point is bigger than the previous point
				points[i][0] = key_x;
				points[i][1] = key_y;
				i--;
			}
		}
	}
	else {

		for (j = 1; j < n; j++) {													// Look all the points and find right place for it
			key_x = points[j][0];
			key_y = points[j][1];
			i = j - 1;

			while (i >= 0 && key_y < points[i][1]) {								// if y value of the point is smaller then previous,
				points[i + 1][0] = points[i][0];									// swap the points(both x and y values) and compare with the other points
				points[i + 1][1] = points[i][1];									// Repeat these steps until y value of the point is bigger than the previous point
				points[i][0] = key_x;
				points[i][1] = key_y;
				i--;
			}
		}
	}
}

MIN bruteForce(int **points, int n) {

	int i, j;
	MIN min;
	float distance;

	min.distance = findDistance(points[0], points[1]);
	min.pointA = points[0];
	min.pointB = points[1];

	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {

			distance = findDistance(points[i], points[j]);
			if (distance < min.distance) {
				min.distance = distance;
				min.pointA = points[i];
				min.pointB = points[j];
			}

		}
	}

	return min;
}

float findDistance(int *pointA, int *pointB) {
	float distance;

	distance = pow((pointA[0] - pointB[0]), 2);
	distance += pow((pointA[1] - pointB[1]), 2);
	return sqrt(distance);
}

MIN findMin(MIN dL, MIN dR) {
	if (dL.distance < dR.distance) { return dL; }
	return dR;
}

int findPoints_aroundMiddle_andSort(int **points, int n, float d, int **aroundPoints) {	 // Find points around the middle and sort them based on y coordinate

	int i;
	float distance;
	int *middlePoint;
	int sizeofAroundPoints = 0;

	middlePoint = points[n / 2];													// Assign the middle point

	for (i = 0; i < n; i++) {

		distance = abs(points[i][0] - middlePoint[0]);								// if distance between x of middlePoint and x of current point
		if (distance < d) {															// smaller than d, add that point to aroundPoints array
			aroundPoints[sizeofAroundPoints] = points[i];
			sizeofAroundPoints++;
		}
	}

	sort(aroundPoints, sizeofAroundPoints, 'y');									// Sort aroundPoints matrix based on y coordinate

	return sizeofAroundPoints;
}


int readFile(char *fileName, int **points) {										// Read point's coordinates from file and return the size of points

	FILE *fi;
	int i;
	char pointA[10], pointB[10];

	fi = fopen(fileName, "r");
	if (fi == NULL) {
		printf("ERROR oppening file\n");
		return -3;
	}

	i = 0;
	while (!feof(fi)) {

		fscanf(fi, "%s %s", pointA, pointB);

		points[i][0] = atoi(pointA);
		points[i][1] = atoi(pointB);

		i++;
	}

	fclose(fi);
	return i;																    // return the array size
}