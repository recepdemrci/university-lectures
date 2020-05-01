#include<stdio.h>
#include<stdlib.h>
#include <time.h> 
#include<math.h>
#include<opencv2/opencv.hpp>
#include<stdbool.h>
#include<string.h>

#define K 32
#define EPSILON 10	

using namespace cv;

// FUNCTIONS PROTOTYPE ------------------------------------
int **K_means(Mat);
double dist(Vec3b, Vec3b);
int findMin(Vec3b, Vec3b *);
bool calculateNewMeans(Mat, int **, Vec3b *);
void convertImage(Mat, int **, Vec3b *);

void Con_Comp_Label(Mat, int **, int, int);
void newEquivalent(int **, int, int);
int getEquivalent(int **, int);
//---------------------------------------------------------


int main(int argc, char **argv) {
	
	char imageName[100];
	int **imageLabel;
	Mat image; 
	
	if (argc != 2) {
		printf("Usage: %s image_path", argv[0]);
		exit(-3);
	}

	strcpy(imageName, argv[1]);
	image = imread(imageName, IMREAD_COLOR);
	
	if (image.empty()) {
		printf("Could not open or find the image");
		return -1;
	}


	imageLabel = K_means(image);
	//imshow("result1", image);
	imwrite("K-means-Clustering_K8.jpg", image);

	Con_Comp_Label(image, imageLabel, image.rows, image.cols);	
	imwrite("Connected-Component-Labeling_K8.jpg", image);
	//imshow("result2", image);
	
	printf("Images are saved on your working directory.");
	waitKey(0);
}


int **K_means(Mat image) {

	int i, j;															// Index variables
	int minIndex;														// Stores the means[] index value which has minimum distance to pixel 
	int **imageLabel;
	Vec3b means[K];
	bool flag = true;

	imageLabel = (int **)malloc(sizeof(int*) * image.rows);				// Define and allocate another image matrix for labelling the pixels
	for (i = 0; i < image.rows; i++) {
		imageLabel[i] = (int *)malloc(sizeof(int) * image.cols);
	}

	srand(time(NULL));
	for (i = 0; i < K; i++) {											// Select random centroids(means). 
		
		means[i][0] = rand() % 255;
		means[i][1] = rand() % 255;
		means[i][2] = rand() % 255;
	}

	while (flag) {				
		
		for (i = 0; i < image.rows; i++) {
			
			for (j = 0; j < image.cols; j++) {
				minIndex = findMin(image.at<Vec3b>(i, j), means);		// Find the minimum distance mean between means(centroids) and pixel
				imageLabel[i][j] = minIndex;							// Write index of mean(which is closest to the pixel value) to the label matrix  
			}
		}
		flag = calculateNewMeans(image, imageLabel, means);				// Calculate new means value until epsilon is bigger than summary of the difference between 
	}																	// new and old mean values			
	
	convertImage(image, imageLabel, means);								// Write mean values to the original image pixels
	
	return imageLabel;
}

double dist(Vec3b pixel, Vec3b mean) {									// Take a pixel and a centroid(mean) as parameters.
																		// Calculates the distance between pixel and mean.
	double distance = 0;
	distance += pow((pixel[0] - mean[0]), 2);		
	distance += pow((pixel[1] - mean[1]), 2);
	distance += pow((pixel[2] - mean[2]), 2);
	distance = sqrt(distance);
	return distance;
}

int findMin(Vec3b pixel, Vec3b *means) {
	
	int i = 0, minIndex	= 0;
	double min;

	min = dist(pixel, means[0]);
	for (i = 1; i < K; i++) {
		
		if (dist(pixel, means[i]) < min) {
			min = dist(pixel, means[i]);
			minIndex = i;
		}
	}
	return minIndex;
}

bool calculateNewMeans(Mat image, int **imageLabel, Vec3b *means) {
	int i, j;
	int meanIndex;		
	double changeOfMeans;
	int count[K];														// To calculate new means[meanIndex] value, first add pixels value to newMeans[meanIndex] 
	Vec3f newMeans[K];													// and divide it to how many pixels value we have in exactly this index.
	
	for (i = 0; i < K; i++) {											// all means(centroids) are set to 0
		newMeans[i][0] = 0;
		newMeans[i][1] = 0;
		newMeans[i][2] = 0;
		count[i] = 1;

	}

	for (i = 0; i < image.rows; i++) {									// Look all pixels value and add them to which mean(centroids) they belong
																		// And during this operation, do not forget to increase count values
		for (j = 0; j < image.cols; j++) {
			
			meanIndex = imageLabel[i][j];

			newMeans[meanIndex][0] += image.at<Vec3b>(i, j)[0];
			newMeans[meanIndex][1] += image.at<Vec3b>(i, j)[1];
			newMeans[meanIndex][2] += image.at<Vec3b>(i, j)[2];
			count[meanIndex]++;
		}
	}

	
	changeOfMeans = 0;
	for (i = 0; i < K; i++) {
		newMeans[i][0] = newMeans[i][0] / count[i];	
		newMeans[i][1] = newMeans[i][1] / count[i];
		newMeans[i][2] = newMeans[i][2] / count[i];
		changeOfMeans += dist(means[i], newMeans[i]);					// Calculate the change between means(centroids) and newMeans(newCentroids)
		
		means[i] = newMeans[i];
	}
		
	printf("Change between means value: %f\n", changeOfMeans);

	if (changeOfMeans <= EPSILON) {										// if the change is smaller than epsilon, function will return false and Clusterin will be finished  
		return false;
	}
	return true;
}

void convertImage(Mat image, int **imageLabel, Vec3b *means) {			// Convert original image pixels to the labeled mean values
	
	int i, j;
	int meanIndex;

	for (i = 0; i < image.rows; i++) {

		for (j = 0; j < image.cols; j++) {

			meanIndex = imageLabel[i][j];

			image.at<Vec3b>(i, j)[0] = means[meanIndex][0];
			image.at<Vec3b>(i, j)[1] = means[meanIndex][1];
			image.at<Vec3b>(i, j)[2] = means[meanIndex][2];
		}
	}
}



void Con_Comp_Label(Mat originalImage, int **image, int image_rows, int image_cols) {

	int i, j;																				// index variables 
	int pixel, left, up, up_and_left, left_label, up_label, up_and_left_label, finalLabel;	// store the current pixel,up,left, left_up pixel values, and their labels
	int **imageLabel;																		// store the labels values of all pixels 
	int **equivalentTable;																	// this matrix has 2 coloums. it stores labels equivalents(relations).
	int labelIndex = 0;


	imageLabel = (int **)malloc(sizeof(int*) * image_rows);									// Define and allocate image Label matrix for labelling the pixels
	for (i = 0; i < image_rows; i++) {
		imageLabel[i] = (int *)malloc(sizeof(int) * image_cols);
	}

	equivalentTable = (int **)malloc(sizeof(int*) * (image_rows*image_cols) );				// Define and allocate Equivalent Table matrix
	for (i = 0; i < (image_rows*image_cols); i++) {
		equivalentTable[i] = (int *)malloc(sizeof(int) * 2);
	}
	
	imageLabel[0][0] = labelIndex;															// Setting the first pixel's label
	newEquivalent(equivalentTable, labelIndex, labelIndex);									// And add this label value to the equivalent(relation) table.(0 -> 0)														
	
	for (j = 1; j < image_cols; j++) {														// Setting the pixel's label of the first row

		if (image[0][j] == image[0][j - 1]) {
			imageLabel[0][j] = labelIndex;
		}
		else {
			labelIndex++;
			imageLabel[0][j] = labelIndex;
			newEquivalent(equivalentTable, labelIndex, labelIndex);
		}
	}

	for (i = 1; i < image_rows; i++) {														// Setting the pixel's label of the first colloums

		if (image[i][0] == image[i-1][0]) {
			imageLabel[i][0] = labelIndex;
		}
		else {
			labelIndex++;
			imageLabel[i][0] = labelIndex;
			newEquivalent(equivalentTable, labelIndex, labelIndex);
		}
	}

	printf("Connected Component Labeling is processing... \n");
	for (i = 1; i < image_rows; i++) {														// Look all the pixels of the image
		for (j = 1; j < image_cols; j++) {

			pixel = image[i][j]; 															// pixel <- current pixel's value
			left = image[i][j - 1];															// left <- left pixel's value
			up = image[i - 1][j];															// up <- up pixel's value
			up_and_left = image[i - 1][j - 1];												// up_and_left <- up_left pixel's value
																							  
			left_label = imageLabel[i][j - 1];												// AND their labels
			up_label = imageLabel[i - 1][j];
			up_and_left_label = imageLabel[i - 1][j - 1];
			
			if (pixel == left) {															// Control the left, up and left_up pixels if its equal to current pixel
				imageLabel[i][j] =  getEquivalent(equivalentTable, left_label);				// And give the label, which belongs to left, up, left_up, to current pixel 
			}
			else if (pixel == up) {
				imageLabel[i][j] = getEquivalent(equivalentTable, up_label);
			}
			else if (pixel == up_and_left) {
				imageLabel[i][j] = getEquivalent(equivalentTable, up_and_left_label);
			}
			else {																			// if current pixel is different from left,up,left_up
				labelIndex++;															   	// Create new label value and give it to the current pixel
				imageLabel[i][j] = labelIndex;												// And save this label to the equivalent table
				newEquivalent(equivalentTable, labelIndex, labelIndex);
			}

			if (left == up && left_label != up_label) {										// if pixels have the same values but different labels,
																							// find and change bigger label in the equivalent table.  Before(5 -> 5) After(5 -> 3)
				if (getEquivalent(equivalentTable,left_label) < getEquivalent(equivalentTable,up_label)) {
					equivalentTable[up_label][1] = getEquivalent(equivalentTable, left_label);
				}	
				else {
					equivalentTable[left_label][1] = getEquivalent(equivalentTable, up_label);
				}
			}
			
		}
	}
	printf("Connected Component Labeling is processing...  \n");
	

	for (i = 0; i < image_rows; i++) {														// Look all the pixels and change their labels by looking at equivalent tables
		for (j = 0; j < image_cols; j++) {													// Change original image colour by looking at label matrix(Give different colours to labels) 
			finalLabel = getEquivalent(equivalentTable, imageLabel[i][j]);

			originalImage.at<Vec3b>(i, j)[0] = (25 * finalLabel) % 255;
			originalImage.at<Vec3b>(i, j)[1] = (50 * finalLabel) % 255;
			originalImage.at<Vec3b>(i, j)[2] = (125 * finalLabel) % 255;
		}
	}
	printf("Connected Component Labeling finished \n");
}

void newEquivalent(int **equivalentTable, int labelA, int labelB) {
	static int tableIndex = 0;

	if (equivalentTable[tableIndex] == NULL ) {
		
		printf("Program terminated.");
		exit(-5);
	}
	
	equivalentTable[tableIndex][0] = labelA;
	equivalentTable[tableIndex][1] = labelB;
	tableIndex++;
}

int getEquivalent(int **equivalentTable, int label) {
	int i = 0;
	
	while (equivalentTable[i][0] != label) {
		i++;
	}

	return equivalentTable[i][1];
}