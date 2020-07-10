#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <opencv2/opencv.hpp>
#include <stdbool.h>
#include <string.h>

#define K 32
#define EPSILON 10	

using namespace cv;

// FUNCTIONS PROTOTYPE ------------------------------------
void K_means(Mat);
double dist(Vec3b, Vec3b);
int findMin(Vec3b, Vec3b *);
bool calculateNewMeans(Mat, int **, Vec3b *);
void convertImage(Mat, int **, Vec3b *);
//---------------------------------------------------------


int main(int argc, char **argv) {

	char imageName[100];
	Mat image;
	printf("> Please ENTER image path(max character 200): ");									   // Take image name and read image
	scanf_s("%200s", imageName, 200);
	image = imread(imageName, IMREAD_COLOR);
	if (image.empty()) {
		printf("[ERROR] : Could not open or find the image\n");
		return -1;
	}

	K_means(image);
	imwrite("K-means-Clustering.jpg", image);
	printf("[INFO] : Images are saved on your working directory\n");


	getchar();
	exit(0);
}



void K_means(Mat image) {

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

	int i = 0, minIndex = 0;
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