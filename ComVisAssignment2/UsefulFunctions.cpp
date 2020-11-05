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