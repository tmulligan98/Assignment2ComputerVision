#include "utilities.h"

bool computeEdges(Mat image) {
	int count = 0;
	Mat gray_image;
	Mat edges;
	//Just chose y = 20 to bisect the edges of the black-white pattern
	int y = 20;

	//1. perform gaussian blur to smooth the image
	//choosing 3x3 size for the time being
	GaussianBlur(image, image, Size(3, 3), 0);

	//2. convert the image to greyscale
	cvtColor(image, gray_image, COLOR_BGR2GRAY);

	//3. use the built in canny function
	//again using 3 as kernel size for time being
	//Canny has thresholding built in. I have set the low threshold (and hence low threshold ratio) to zero
	Canny(gray_image, edges, 80, 240);

	//now need to work through the outputted image and count the edges produced by canny.
	//imshow("Binary Edge image", edges);

	for (int x = 0; x < edges.cols; x++) {
		if (edges.at<uchar>(y, x) > 0) count++;
	}
	//cout << count << endl;

	//From looking at the output of the count, we get 10 edges for a empty postbox.

	if (count != 10) return true;
	return false;

}