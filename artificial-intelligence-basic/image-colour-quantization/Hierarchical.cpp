//
//	Hierarchical clustering Agglomerative
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <opencv2/opencv.hpp>
#include <stdbool.h>
#include <string.h>

using namespace cv;

// FUNCTIONS PROTOTYPE ------------------------------------
void hier(Mat image, int);
void changeColor(Vec3b, Vec3b, Mat image);
//---------------------------------------------------------

int main(int argc, char **argv) {

	char imageName[200];
	Mat image;
	printf("> Please ENTER image path(max character 200): ");									   // Take image name and read image
	scanf_s("%200s", imageName, 200);
	image = imread(imageName, IMREAD_COLOR);
	if (image.empty()) {
		printf("[ERROR] : Could not open or find the image\n");
		return -1;
	}
	int EPSILON;																					// Take EPSILON value
	printf("> Please ENTER the expected difference value: ");
	scanf("%d", &EPSILON);
				
				
	hier(image, EPSILON);																			// Call Hierarchical clustering function


	imwrite("-Hierarchical-Clustered-image.png", image);											// export clustered image
	printf("[INFO] : Images are saved on your working directory\n");

	getchar();
	exit(0);
}

// AGGLOMERATIVE
// given a dataset(d1, d2, d3, ....dN) of size N
// # compute the distance matrix
// for i = 1 to N :
//		# as the distance matrix is symmetric about
//		# the primary diagonal so we compute only lower
//		# part of the primary diagonal
//		for j = 1 to i :
//			dis_mat[i][j] = distance[di, dj]
// each data point is a singleton cluster
// repeat
//		merge the two cluster having minimum distance
//		update the distance matrix
// untill only a single cluster remains

void hier(Mat image, int EPSILON) {

	int p1x, p1y, p2x, p2y;					// Index variables for two pixels
	int s_p1x, s_p1y, s_p2x, s_p2y;			// 2 pixels with maximum similarity
	int mostSimy, simy;						// similarity value to compare with each other and EPSILON
	Vec3b p1, p2;							// Holds most similat two pixels' data
	bool flag = true;

	while (flag) {
		mostSimy = 765;
		for (p1x = 0; p1x < image.rows; p1x++) {
			for (p1y = 0; p1y < image.cols; p1y++) {
				for (p2x = p1x; p2x < image.rows; p2x++) {
					for (p2y = p1y + 1; p2y < image.cols; p2y++) {
						if (image.at<Vec3b>(p1x, p1y) == image.at<Vec3b>(p2x, p2y)) {					// If 2 pixels are same do nothing
							// Pixeller ayniytsa bir sey yapma											//  because they considered to be in the same cluster
							// printf("(%d, %d)~(%d, %d)\n\n", p1x, p1y, p2x, p2y);
						}
						else {																			// If 2 pixels are not some find how similiar are they
							// Farkiysa benzerliklerini hesapla
							simy = 0;
							simy += abs(image.at<Vec3b>(p1x, p1y)[0] - image.at<Vec3b>(p2x, p2y)[0]);
							simy += abs(image.at<Vec3b>(p1x, p1y)[1] - image.at<Vec3b>(p2x, p2y)[1]);
							simy += abs(image.at<Vec3b>(p1x, p1y)[2] - image.at<Vec3b>(p2x, p2y)[2]);
							if (simy < mostSimy) {														// Find the most similar 2 pixels in the whole image
								mostSimy = simy;														//  and save them as p1 and p2 to send to changeColor function
								s_p1x = p1x;
								s_p1y = p1y;
								s_p2x = p2x;
								s_p2y = p2y;
								p1 = image.at<Vec3b>(p1x, p1y);
								p2 = image.at<Vec3b>(p2x, p2y);
								//printf("(%d, %d)~(%d, %d)\n\n", p1x, p1y, p2x, p2y);
							}
						}
					}
				}
			}
			system("CLS");																				// Display the progress
			printf("Scaning the image...\n");
			printf("%%%d\n", (p1x * 100 / image.rows));
			printf("Curr / Target Compression\n");
			printf("%4d / %d\n", mostSimy, EPSILON);
		}
		
 		if (mostSimy >= EPSILON) {																		// If most similiar two pixels are not similiar enough
			flag = false;																				//  to form a cluster, end the algorithm
		}

		changeColor(p1, p2, image);																		// Make a cluster out of p1 and p2.
																										//  every pixel that is same with p1 or p2 joins the cluseter
	}	
}

// Gets 2 pixels
//  p1 and every pixel that have the same color as p1 and
//  p2 and every pixel that have the same color as p2
//  makes a cluster and all the cluster takes a avarage color
//  calculated by the colors of p1 and p2
void changeColor(Vec3b p1, Vec3b p2, Mat image) {
		
	int px, py;

	// Calculate the new cluster color from p1 and p2's colors
	Vec3b newPixel;
	newPixel[0] = (p1[0] + p2[0]) / 2;	// Blue
	newPixel[1] = (p1[1] + p2[1]) / 2;	// Green
	newPixel[2] = (p1[2] + p2[2]) / 2;	// Red

	// Recolor the whole cluster
	for (px = 0; px < image.rows; px++) {
		for (py = 0; py < image.cols; py++) {
			if (image.at<Vec3b>(px, py) == p1 )
				image.at<Vec3b>(px, py) = newPixel;
			if (image.at<Vec3b>(px, py) == p2 ) 
				image.at<Vec3b>(px, py) = newPixel;
		}
	}
}
