#include "utilities.h"

bool* computeEdges(Mat images[NUMBER_BOXES], Mat &combined_binary) {

	//Compute edges, return bool-pointer indicating which boxes have post


	int counts1[NUMBER_BOXES] = {};
	int counts2[NUMBER_BOXES] = {};
	int counts3[NUMBER_BOXES] = {};
	int counts4[NUMBER_BOXES] = {};

	Mat gray_images[6];
	Mat binary_edges[6];
	Mat combined;
	bool post_present[6];
	//Just chose y = 20 to bisect the edges of the black-white pattern
	int y = 10;
	int i = 0;

	Scalar colour(255);
	
	//Perform Canny on each perspective transform
	for (i = 0; i < NUMBER_BOXES; i++) { 
		
		//1. perform gaussian blur to smooth the image
		//choosing 3x3 size for the time being
		GaussianBlur(images[i], images[i], Size(3, 3), 0); 

		//2. convert the image to greyscale
		cvtColor(images[i], gray_images[i], COLOR_BGR2GRAY);

		//3. use the built in canny function
		//Canny has thresholding built in. 
		Canny(gray_images[i], binary_edges[i], 50, 110);


	}

	//Combine the sections to one
	combined = RecreateFrame(binary_edges);
	
	//now need to work through the outputted image and count the edges produced by canny.
	//Method: 
	//1) Take four horizontal lines across each postbox
	//2) Count the number of edges the horizontal lines bisect
	//3) Perfectly visible post box lines will give 10 lines only
	//Since things are rarely perfect, I have a "closeEnough" function

	//Potential improvement: When counting an edge, in order to be stricter about what we count, check if the pixels to the left and right are 0.
	//But this works pretty well

	for (i = 0; i < NUMBER_BOXES; i++) {
		for (int x = 0; x < binary_edges[i].cols; x++) {
			if (binary_edges[i].at<uchar>(10, x) > 0) counts1[i]++;
			if (binary_edges[i].at<uchar>(20, x) > 0) counts2[i]++;
			if (binary_edges[i].at<uchar>(30, x) > 0) counts3[i]++;
			if (binary_edges[i].at<uchar>(40, x) > 0) counts4[i]++;

		}

		if (closeEnough(counts1[i], counts2[i], counts3[i], counts4[i])) post_present[i] = false;
		else post_present[i] = true;
	}
	
	//cout << counts1[0] << " " << counts2[0] << " " << counts1[1] << " " << counts2[1] << endl;


	if(post_present[0]) putText(combined, "Post", Point2f(0, 10), FONT_HERSHEY_SIMPLEX, 0.4, colour);	//Top left...
	if(post_present[1]) putText(combined, "Post", Point2f(90, 10), FONT_HERSHEY_SIMPLEX, 0.4, colour);
	if(post_present[2]) putText(combined, "Post", Point2f(0, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
	if(post_present[3]) putText(combined, "Post", Point2f(90, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
	if(post_present[4]) putText(combined, "Post", Point2f(0, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
	if(post_present[5]) putText(combined, "Post", Point2f(90, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);	//Bottom right...

	
	
	//imshow("Binary Edge image", combined);
	combined_binary = combined;

	//From looking at the output of the count, we get 10 edges for a empty postbox.

	return post_present;

}