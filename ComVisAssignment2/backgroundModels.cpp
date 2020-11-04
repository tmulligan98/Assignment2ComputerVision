#include "utilities.h"

bool detectMovement(Mat frame, Ptr<BackgroundSubtractor> background_model) {

	Mat foreground_mask;
	Mat gray_frame;

	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	
	//Gernerate foreground mask and update background model
	background_model->apply(gray_frame, foreground_mask, 0.3);

	imshow("Foreground mask", foreground_mask);

	//Count number of countours. If above a cetain threshold we have movement
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy_of_contours;
	findContours(foreground_mask, contours, hierarchy_of_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	//imshow("Mask", foreground_mask);
	cout << contours.size() << endl;
	if (contours.size() > 40) return true;
		return false;
}