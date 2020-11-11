#include "utilities.h"

Mat RecreateFrame(Mat perspectives[6]) {
	int spacing = 5;
	Mat output(270 + spacing*2, 180 + spacing, perspectives[0].type());
	output.setTo(Scalar(255, 255, 255));
	Mat regionsOfInterest[6];

	//Define the regions of interest for each section of the frame.
	regionsOfInterest[0] = output(Rect(0,0, 90, 90));	//Top left
	regionsOfInterest[1] = output(Rect(95, 0, 90, 90)); //Top Right
	regionsOfInterest[2] = output(Rect(0, 95, 90, 90)); //Middle left
	regionsOfInterest[3] = output(Rect(95, 95, 90, 90)); //Middle right
	regionsOfInterest[4] = output(Rect(0, 190, 90, 90)); //Bottom right
	regionsOfInterest[5] = output(Rect(95, 190, 90, 90)); // Bottom left

	for (int i = 0; i < 6; i++) perspectives[i].copyTo(regionsOfInterest[i]);

	//for (int i = 0; i < 6; i++) regionsOfInterest[i].copyTo(output);

	return output;
}

bool closeEnough(int a, int b, int c, int d) {

	int correct = 0;

	
	if (a >= 10 && a <= 14) correct++;
	if (b >= 10 && b <= 14) correct++;
	if (c >= 10 && c <= 14) correct++;
	if (d >= 10 && d <= 14) correct++;

	//If two or more of the horizontal lines have counted between 10 and 14 edges, then return true.
	if (correct >= 2) return true;
	return false;

}

void createMask(Mat frame, Point2f sources[][4]) {
	Mat regionsOfInterest[6];
	
	//Otsu threshold the frame.
	frame = otsuThresholding(frame);

	//Create empty mask with same dimensions as frame
	Mat mask1 = Mat::zeros(frame.rows, frame.cols, frame.type());
	//mask1.setTo(Scalar(255));
	Mat mask2;

	//Copy frame to mask.
	frame.copyTo(mask2);

	//Define regions of interest for each zone to mask off.
	//Rect2f(topleft(of region) x, topleft y, width, height)
	regionsOfInterest[0] = mask1(Rect2f(sources[0][0].x, sources[0][0].y, sources[0][1].x - sources[0][0].x, sources[0][2].y - sources[0][0].y)); //Top left
	regionsOfInterest[1] = mask1(Rect2f(sources[1][0].x, sources[1][0].y, sources[1][1].x - sources[1][0].x, sources[1][2].y - sources[1][0].y));
	regionsOfInterest[2] = mask1(Rect2f(sources[2][0].x, sources[2][0].y, sources[2][1].x - sources[2][0].x, sources[2][2].y - sources[2][0].y));
	regionsOfInterest[3] = mask1(Rect2f(sources[3][0].x, sources[3][0].y, sources[3][1].x - sources[3][0].x, sources[3][2].y - sources[3][0].y));
	regionsOfInterest[4] = mask1(Rect2f(sources[4][0].x, sources[4][0].y, sources[4][1].x - sources[4][0].x, sources[4][2].y - sources[4][0].y));
	regionsOfInterest[5] = mask1(Rect2f(sources[5][0].x, sources[5][0].y, sources[5][1].x - sources[5][0].x, sources[5][2].y - sources[5][0].y)); // Bottom right

	//Now have zero regions for black-white patterns

	//Invert regions, combine to mask.
	for (int i = 0; i < NUMBER_BOXES; i++) bitwise_not(regionsOfInterest[i], regionsOfInterest[i], noArray());

	//Invert overall mask now.
	bitwise_not(mask1, mask1, noArray());
	bitwise_and(frame, mask1, mask1, noArray());
	
	//Now have image with mostly just frame of postboxes!



	imshow("Masked binary image", mask1);


}

bool isEmpty(bool arr[NUMBER_BOXES]) {

	for (int i = 0; i < NUMBER_BOXES; i++) {
		if (arr[i] == true) return false;
	}
	return true;
}