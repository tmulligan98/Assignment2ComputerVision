#include "utilities.h"

bool detectMovementGMM(Mat frame, Ptr<BackgroundSubtractor> background_model) {
	//NOT USED//
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

bool detectMovement_FrameDifference(Mat cur_frame, Mat &prev_frame) {

	//Will detect motion. But what about someone who's just decided to park themselves infront of the camera?
	//Well we won't detect movement. So we'll have to check if we can see the stripes... 

	static Mat diff_frame;

	cvtColor(cur_frame, cur_frame, COLOR_BGR2GRAY);

	if(!prev_frame.empty()){

		absdiff(cur_frame, prev_frame, diff_frame);
		//Threshold the difference image.
		threshold(diff_frame, diff_frame, 50, 255, THRESH_BINARY);

		//Having looked at the amount of non-zero values for frames with movement...
		//cout << "\t" << countNonZero(diff_frame) << endl;

		imshow("Difference image", diff_frame);
		prev_frame = cur_frame;
		
		if (countNonZero(diff_frame) >= 500) return true;


		//No movement? Check if we can see the black and white stripes. If not we have a problem.
		return false;


	}
	else {
		//This will occur from the first frame being processed.
		prev_frame = cur_frame;
		return false;
	}
	


}