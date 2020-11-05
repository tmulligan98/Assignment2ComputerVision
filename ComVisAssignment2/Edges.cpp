#include "utilities.h"

bool* computeEdges(Mat images[6]) {
	int counts[6] = {};
	Mat gray_images[6];
	Mat binary_edges[6];
	Mat combined;
	bool post_present[6];
	//Just chose y = 20 to bisect the edges of the black-white pattern
	int y = 20;
	int i = 0;

	
	for (i = 0; i < 6; i++) { 
		
		//1. perform gaussian blur to smooth the image
		//choosing 3x3 size for the time being
		GaussianBlur(images[i], images[i], Size(3, 3), 0); 

		//2. convert the image to greyscale
		cvtColor(images[i], gray_images[i], COLOR_BGR2GRAY);

		//3. use the built in canny function
		//again using 3 as kernel size for time being
		//Canny has thresholding built in. I have set the low threshold (and hence low threshold ratio) to zero
		Canny(gray_images[i], binary_edges[i], 90, 270);

	}

	//combined = RecreateFrame(binary_edges);
	
	//now need to work through the outputted image and count the edges produced by canny.
	//imshow("Binary Edge image", combined);
	for (i = 0; i < 6; i++) {
		for (int x = 0; x < binary_edges[i].cols; x++) {
			if (binary_edges[i].at<uchar>(y, x) > 0) counts[i]++;
		}

		if (counts[i] == 10 || counts[i] == 11) post_present[i] = true;
	}
	
	//cout << counts[0] << " " << counts[1] << " " << counts[2] << " " << counts[3] << " " << counts[4] << " " << counts[5] << endl;

	//From looking at the output of the count, we get 10 edges for a empty postbox.

	return post_present;

}