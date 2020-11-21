#include "utilities.h"

Mat RecreateFrame(Mat perspectives[NUMBER_BOXES]) {
	//Join 6 90x90 sized images to one larger display image
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

	for (int i = 0; i < NUMBER_BOXES; i++) perspectives[i].copyTo(regionsOfInterest[i]);

	

	return output;
}

bool closeEnough(int a, int b, int c, int d) {

	//If 3/4 bisectors have found enough lines, then we say that post is not present

	int correct = 0;

	
	if (a >= 10 && a <= 14) correct++;
	if (b >= 10 && b <= 14) correct++;
	if (c >= 10 && c <= 14) correct++;
	if (d >= 10 && d <= 14) correct++;

	//If two or more of the horizontal lines have counted between 10 and 14 edges, then return true.
	if (correct >= 3) return true;
	return false;

}

void createMask(Mat frame, Point2f sources[][4]) {
	//Proof of concept, not actually used in solution.
	//Can be uncommented in VideoDemo to have a look at what it does

	Mat regionsOfInterest[NUMBER_BOXES];
	
	//Otsu threshold the frame.
	frame = otsuThresholding(frame);

	//Create empty mask with same dimensions as frame
	Mat mask1 = Mat::zeros(frame.rows, frame.cols, frame.type());
	//mask1.setTo(Scalar(255));
	//Mat mask2;

	//Copy frame to mask.
	//frame.copyTo(mask2);

	//Define regions of interest for each zone to mask off.
	regionsOfInterest[0] = mask1(Rect2f(sources[0][0].x, sources[0][0].y, sources[0][1].x - sources[0][0].x, sources[0][2].y - sources[0][0].y)); //Top left
	regionsOfInterest[1] = mask1(Rect2f(sources[1][0].x, sources[1][0].y, sources[1][1].x - sources[1][0].x, sources[1][2].y - sources[1][0].y));
	regionsOfInterest[2] = mask1(Rect2f(sources[2][0].x, sources[2][0].y, sources[2][1].x - sources[2][0].x, sources[2][2].y - sources[2][0].y));
	regionsOfInterest[3] = mask1(Rect2f(sources[3][0].x, sources[3][0].y, sources[3][1].x - sources[3][0].x, sources[3][2].y - sources[3][0].y));
	regionsOfInterest[4] = mask1(Rect2f(sources[4][0].x, sources[4][0].y, sources[4][1].x - sources[4][0].x, sources[4][2].y - sources[4][0].y));
	regionsOfInterest[5] = mask1(Rect2f(sources[5][0].x, sources[5][0].y, sources[5][1].x - sources[5][0].x, sources[5][2].y - sources[5][0].y)); // Bottom right

	//Now have zero regions for black-white patterns
	
	//Invert regions, combine to mask.
	for (int i = 0; i < NUMBER_BOXES; i++) bitwise_not(regionsOfInterest[i], regionsOfInterest[i], noArray());
	//imshow("Mask", mask1);
	//Invert overall mask now.
	bitwise_not(mask1, mask1, noArray());
	bitwise_and(frame, mask1, mask1, noArray());
	
	//Now have image with mostly just frame of postboxes!

	imshow("Masked binary image", mask1);


}

//Handy utility function
bool isEmpty(bool arr[NUMBER_BOXES]) {

	for (int i = 0; i < NUMBER_BOXES; i++) {
		if (arr[i] == true) return false;
	}
	return true;
}


//Attempt at using Template Matching to find the locations... Some interesting results
void findLocations(Mat frame, Mat template_image) {
	char c;
	Mat grey_image, binary_image, hls_image, opened_image, line_display;
	int bisectors[10];
	Mat display_image;
	frame.copyTo(display_image);

	//Convert to HLS image and equalise luminance channel
	cvtColor(frame, hls_image, COLOR_BGR2HLS);

	vector <Mat> channels(hls_image.channels());
	split(hls_image, channels);
	equalizeHist(channels[1], channels[1]);
	merge(channels, hls_image);
	//Convert back to RGB
	cvtColor(hls_image, frame, COLOR_HLS2BGR);


	imshow("Equalised, thresholded, opened",frame);
	


	int result_x = frame.cols - template_image.cols + 1;
	int result_y = frame.rows - template_image.rows + 1;

	Mat result(result_y, result_x, CV_32FC1);

	matchTemplate(frame, template_image, result, cv::TM_CCOEFF_NORMED);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	//Now find best match locations
	double min_val, max_val;
	Point min_location, max_location, match_location;

	minMaxLoc(result, &min_val, &max_val, &min_location, &max_location, Mat());
	//Since using COEFF_NORMED match location =  minimum location
	match_location = min_location;

	resize(result, result, Size(frame.cols, frame.rows));

	//Put bounding rectangles over locations.
	rectangle(result, match_location, Point(match_location.x + template_image.cols, match_location.y + template_image.rows), CV_RGB(255,0,0));
	rectangle(display_image, match_location, Point(match_location.x + template_image.cols, match_location.y + template_image.rows), CV_RGB(255, 0, 0));


	
	imshow("Template", template_image);
	imshow("result", result);
	imshow("image_window", display_image);

	c = cv::waitKey();
	cv::destroyAllWindows();



}

//Only used above
void equaliseShadows(Mat &frame) {

	Mat hls_image;
	
	//Convert to HLS image
	cvtColor(frame, hls_image, COLOR_BGR2HLS);

	//Using the luminance channel, equalise the brightness.
	vector <Mat> channels(hls_image.channels());
	split(hls_image, channels);
	equalizeHist(channels[1], channels[1]);
	merge(channels, hls_image);

	//Convert back to original frame
	cvtColor(hls_image, frame, COLOR_HLS2BGR);
}



//Last minute change. Just clears up ProcessVideo.
//Performs perspective transformations on each post-box base.
void perspectiveTransforms(Mat frame, Mat perspective_matrices[], Mat perspective_warped_images[], Point2f sources[][4], Point2f destinations[]) {

	perspective_warped_images[0] = Mat::zeros(90, 90, frame.type());
	perspective_matrices[0] = getPerspectiveTransform(sources[0], destinations);
	warpPerspective(frame, perspective_warped_images[0], perspective_matrices[0], perspective_warped_images[0].size());

	perspective_warped_images[1] = Mat::zeros(90, 90, frame.type());
	perspective_matrices[1] = getPerspectiveTransform(sources[1], destinations);
	warpPerspective(frame, perspective_warped_images[1], perspective_matrices[1], perspective_warped_images[1].size());

	perspective_warped_images[2] = Mat::zeros(90, 90, frame.type());
	perspective_matrices[2] = getPerspectiveTransform(sources[2], destinations);
	warpPerspective(frame, perspective_warped_images[2], perspective_matrices[2], perspective_warped_images[2].size());

	perspective_warped_images[3] = Mat::zeros(90, 90, frame.type());
	perspective_matrices[3] = getPerspectiveTransform(sources[3], destinations);
	warpPerspective(frame, perspective_warped_images[3], perspective_matrices[3], perspective_warped_images[3].size());

	perspective_warped_images[4] = Mat::zeros(90, 90, frame.type());
	perspective_matrices[4] = getPerspectiveTransform(sources[4], destinations);
	warpPerspective(frame, perspective_warped_images[4], perspective_matrices[4], perspective_warped_images[4].size());

	perspective_warped_images[5] = Mat::zeros(90, 90, frame.type());
	perspective_matrices[5] = getPerspectiveTransform(sources[5], destinations);
	warpPerspective(frame, perspective_warped_images[5], perspective_matrices[5], perspective_warped_images[5].size());

}