#include "utilities.h"

int computeEdges(Mat image) {
	Mat gray_image;
	Mat edges;

	//1. Perform Gaussian Blur to smooth the image
	//Choosing 3x3 size for the time being
	GaussianBlur(image, image, Size(3, 3), 0);

	//2. Convert the image to greyscale
	cvtColor(image, gray_image, COLOR_BGR2GRAY);

	//3. Use the built in Canny Function
	//Again using 3 as kernel size for time being
	Canny(gray_image, edges, 0, 0, 3);

	//Now need to work through the outputted image and count the edges produced by Canny.

}