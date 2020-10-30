// ComVisAssignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "utilities.h"
#include <iostream>
#include <fstream>
#include <list>
#include <string>


int main(int argc, const char** argv)
{

	const char* file_location = "Media/";
	const char* imageFile = "TrinityCampanile1.JPG";
	const char* videofile = "PostboxesWithLines.avi";
	
	int current_frame_count = 0;
	//Get image file and load.
	Mat image;
	string imgfilename(file_location);
	imgfilename.append(imageFile);
	image = imread(imgfilename, IMREAD_COLOR);

	if (image.empty()) {
		cout << "Could not read image" << endl;
		return -1;
	}

	//Get video file and load.
	VideoCapture video;
	string filename(file_location);
	filename.append(videofile);
	video.open(filename);
	//Working frame by frame, capture video.







	cout << "Press 1 to play video" << endl;
	

	while (1) {

		Mat frames;
		video >> frames;

		if (frames.empty()) {
			break;
		}

		imshow("Welcome", frames);
		while (current_frame_count < 100000000) {
			current_frame_count++;
		}
		current_frame_count = 0;

		int choice = waitKey(1);
		if (choice == 1) break;
	}


	video.release();
	destroyAllWindows();
	//
	//do {
	//	imshow("Welcome", image);
	//	
	//	cv::destroyAllWindows();

	//	switch (choice) {
	//	case '1':
	//		while (choice == 1) {
	//			
	//		}
	//		
	//	default:
	//		break;
	//	}
	//} while (choice != 'x');
	
	
	



	return 0;

}


