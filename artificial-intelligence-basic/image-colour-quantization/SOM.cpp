#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <opencv2/opencv.hpp>
#include <stdbool.h>
#include <string.h>

// TODO: Sabitleri ayarla 
#define T 512*50
#define K 16

using namespace cv;

// FUNCTIONS PROTOTYPE ----------------------------------------------
Vec3f **SOM(int, int, Mat, int, float, float);
Vec3f **initWeight(int, int);
Vec3f getRandomInput(Mat);
int *findBMU(Vec3f, Vec3f **, int, int);
void updateWeights(Vec3f, Vec3f **, int, int, int *, float, float);
float neighbourhoodFunc(int, double, float);
float calcSigma(int, float);
float calcLearningRate(int, float);
double dist(Vec3f, Vec3f);
Mat dataSampling(Mat);
void convertData(Mat, Vec3f **, int, int);
//-------------------------------------------------------------------


// TODO: rapora makaleyi ekle
int main(int argc, char **argv) {

	char imageName[200];
	int iterationSize;
	Mat image, normalizedData, sampleData, resultImage;
	Vec3f **neurons;

	printf("> Please ENTER image path(max character 200): ");									   // Take image name and read image
	scanf_s("%200s", imageName, 200);
	image = imread(imageName, IMREAD_COLOR);
	if (image.empty()) {
		printf("[ERROR] : Could not open or find the image\n");
		return -1;
	}

	normalize(image, normalizedData, 0, 1, NORM_MINMAX, CV_32F);			   // Normalize image pixels in range 0.0 - 1.0
	sampleData = dataSampling(normalizedData);								   // Subsampling data in order not to look similar pixels
	iterationSize = sampleData.rows * sampleData.cols * 1000;				   // Calculate iteration size

	neurons = SOM(sqrt(K), sqrt(K), sampleData, iterationSize, 0.1, 0.5);	   // Initialize SOM Clustering and run

	convertData(normalizedData, neurons, sqrt(K), sqrt(K));				       // Convert image pixel values to their neurons values
	normalize(normalizedData, resultImage, 0, 255, NORM_MINMAX, CV_32F);	   // Convert to real pixels value
	imwrite("SOM-Clustering.jpg", resultImage);		     					   // Save result image
	printf("[INFO] : Images are saved on your working directory\n");

	getchar();
	return 0;
}



/*
x: rows of FeatureMap
y: columns of FeatureMap
data: image (Pixel values must be in range 0.0 - 1.0)
sigma: For Collaboration (it will be getting smaller)
learningRate: For Weight Update (it will be getting smaller)
return: Clustered data
*/
Vec3f **SOM(int x, int y, Mat data, int iterationNum, float sigma, float learninRate) {
	int i;
	int *indexBMU;
	Vec3f **neurons;
	Vec3f input;

	neurons = initWeight(x, y);												   // Create neurons and initialize the weights
	for (i = 0; i < iterationNum; i++) {
		if (i % 50000 == 0) {
			printf("[INFO] : Neurons are training %% %.2f\n", ((float)i / iterationNum) * 100);
		}
		input = getRandomInput(data);
		indexBMU = findBMU(input, neurons, x, y);
		updateWeights(input, neurons, x, y, indexBMU, sigma, learninRate);
	}
	printf("[INFO] : Training is completed\n");
	return	neurons;
}



/*
Initialize the weight of the nneurons
-rows: number of rows in FeatureMap
-cols: number of columns in FeaturMap
-return: FeatureMap after initialize the weight
*/
Vec3f **initWeight(int rows, int cols) {
	int i = 0, j = 0;
	Vec3f **neurons;


	neurons = (Vec3f **)malloc(sizeof(Vec3f*) * rows);						   // Define and allocate neurons(featureMap)
	for (i = 0; i < rows; i++) {
		neurons[i] = (Vec3f *)malloc(sizeof(Vec3f) * cols);
	}
	if (neurons == NULL) {
		printf("[ERROR]: Allocation error occurred\n");
		exit(-10);
	}

	srand(time(NULL));
	for (i = 0; i < rows; i++) {											   // Initialize weights randomly in range 0.0 - 1.0
		for (j = 0; j < cols; j++) {
			neurons[i][j][0] = (float)rand() / RAND_MAX;
			neurons[i][j][1] = (float)rand() / RAND_MAX;
			neurons[i][j][2] = (float)rand() / RAND_MAX;
		}
	}
	return neurons;
}


/*
Chose a random input from data
-data: in this example it is a image
-return: chosen vector for input
*/
Vec3f getRandomInput(Mat data) {
	int i, j;
	bool flag = true;
	Vec3f randInput;

	while (flag) {
		srand(time(NULL));
		i = rand() % data.rows;
		j = rand() % data.cols;
		randInput = data.at<Vec3f>(i, j);

		if (randInput[0] < 0 || randInput[0] > 1 ||
			randInput[1] < 0 || randInput[1] > 1 || 
			randInput[2] < 0 || randInput[2] > 1) {
			flag = true;
		}
		else {
			flag = false;
		}
	}
	
	return randInput;
}


/*
Find Best Matching Unit(BMU)
-x: randomly chosen input vector
-neurons: neuron matrix(Feature Map)
-rows, cols: neurons matrix rows and columns
-return: indexes of a neuron which is winners
*/
int *findBMU(Vec3f x, Vec3f **neurons, int rows, int cols) {
	int i, j;
	double currentDist;
	double minDist = 100000;
	int *winnerIndex;

	winnerIndex = (int *)malloc(sizeof(int) * 2);
	if (winnerIndex == NULL) {
		printf("[ERROR]: Allocation error occurred\n");
		exit(-11);
	}

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			currentDist = dist(x, neurons[i][j]);
			if (currentDist < minDist) {
				minDist = currentDist;
				winnerIndex[0] = i;
				winnerIndex[1] = j;
			}
		}
	}

	if (minDist == 100000) {
		printf("[ERROR] : Something wrong, while finding BMU\n");
		exit(-12);
	}
	return winnerIndex;
}


/*
Update the weights of neurons based on neigbourhood
-x: randomly chosen input vector
-neurons: neurons matrix(Feature Map)
-winnerIndex: indexes(row, column) of winner neuron
-sigma: sigma value for gaussian distrubitian while computing radius of neighbourhood (It will be getting smaller)
-learningRate: step size of updating weight (It will be getting smaller)
-return: void
*/
void updateWeights(Vec3f x, Vec3f **neurons, int rows, int cols, int *indexBMU, float sigma0, float learninRate0) {
	int i, j;
	static int iterationNumber = 0;
	float h;
	float learningRate;
	double distance;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			distance = dist(neurons[indexBMU[0]][indexBMU[1]], neurons[i][j]);
			h = neighbourhoodFunc(iterationNumber, distance, sigma0);

			if (h > 0) {
				learningRate = calcLearningRate(iterationNumber, learninRate0);
				neurons[i][j][0] = (neurons[i][j][0] + (learningRate * h * (x[0] - neurons[i][j][0])));
				neurons[i][j][1] = (neurons[i][j][1] + (learningRate * h * (x[1] - neurons[i][j][1])));
				neurons[i][j][2] = (neurons[i][j][2] + (learningRate * h * (x[2] - neurons[i][j][2])));
			}
		}
	}
	iterationNumber++;
}


/*
Calculate neighbourhood value (Influence rate)
-iterationNumber: number of iteration
-dist: distance between BMU and current neurons
-sigma0: initial value of sigma
-return: influence rate
*/
float neighbourhoodFunc(int iterationNumber, double dist, float sigma0) {
	double result = 0;
	float sigma;

	sigma = calcSigma(iterationNumber, sigma0);
	result = exp((-1 * pow(dist, 2)) / (2 * pow(sigma, 2)));
	return (float)result;
}


/*
Calculate new sigma value based on iteration number
-ietrationNumber: number of iteration
-sigma0: initial sigma value
return: new sigma
*/
float calcSigma(int iterationNumber, float sigma0) {
	double result = 0;

	result = exp(-(double)iterationNumber / T);
	result *= sigma0;
	return (float)result;
}


/*
Calculate new learning rate value based on iteration number
-iterationNumber: number of iteration
-learningRate0: initial learningRate value
return: new learninn rate
*/
float calcLearningRate(int iterationNumber, float learningRate0) {
	double result = 0;

	result = exp((double)(-1 * iterationNumber) / T);
	result *= learningRate0;
	return (float)result;
}


// Calculate the distance between two vector
double dist(Vec3f x, Vec3f y) {
	double distance = 0;
	distance += pow((x[0] - y[0]), 2);
	distance += pow((x[1] - y[1]), 2);
	distance += pow((x[2] - y[2]), 2);
	distance = sqrt(distance);
	return distance;
}


/*
Subsampling data in order not to look at similar pixels
-data: our data (in ths example it is an image)
return: void
*/
Mat dataSampling(Mat data) {
	int i, j, m, k;
	int samplingRate;


	samplingRate = (int)sqrt(data.cols);
	int _row = data.rows / samplingRate;
	int _col = data.cols / samplingRate;
	Mat newData = Mat(_row + 1, _col + 1, CV_32FC(3));

	i = 0;
	m = 0;
	while (i < data.rows && m < newData.rows) {
		j = 0;
		k = 0;
		while (j < data.cols && k < newData.cols) {
			newData.at<Vec3f>(m, k) = data.at<Vec3f>(i, j);
			j += samplingRate;
			k++;
		}

		i += samplingRate;
		m++;
	}
	printf("[INFO]: Data sampling completed\n");
	return newData;
}


/*
Convert data from original value to calculated neurons value(and multiply with 255 for real pixel value)
-data: original data in range (0.0 - 1.0)
-neurons: trained neurons
return: void
*/
void convertData(Mat data, Vec3f **neurons, int x, int y) {
	int i, j, _i, _j;
	int *indexBMU;

	for (i = 0; i < data.rows; i++) {										   // Convert all data value from original to labeled value
		for (j = 0; j < data.cols; j++) {
			indexBMU = findBMU(data.at<Vec3f>(i, j), neurons, x, y);
			_i = indexBMU[0];
			_j = indexBMU[1];

			data.at<Vec3f>(i, j)[0] = neurons[_i][_j][0];
			data.at<Vec3f>(i, j)[1] = neurons[_i][_j][1];
			data.at<Vec3f>(i, j)[2] = neurons[_i][_j][2];
		}
	}
}