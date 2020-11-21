// ComVisAssignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Amount of files is probably unecessary, but for my own sake I've kept everything separated out for future editing


#include "utilities.h"
#include "Video.h"
#include <iostream>
#include <fstream>
#include <list>
#include <string>

//This seems like a cumbersome way to do it?
#define NUMBER_OF_POSTBOXES 6
#define POSTBOX_TOP_LEFT_COLUMN 0
#define POSTBOX_TOP_LEFT_ROW 1
#define POSTBOX_TOP_RIGHT_COLUMN 2
#define POSTBOX_TOP_RIGHT_ROW 3
#define POSTBOX_BOTTOM_LEFT_COLUMN 4
#define POSTBOX_BOTTOM_LEFT_ROW 5
#define POSTBOX_BOTTOM_RIGHT_COLUMN 6
#define POSTBOX_BOTTOM_RIGHT_ROW 7

int PostboxLocations[NUMBER_OF_POSTBOXES][8] = {
	{ 26, 113, 106, 113, 13, 133, 107, 134 },
	{ 119, 115, 199, 115, 119, 135, 210, 136 },
	{ 30, 218, 108, 218, 18, 255, 109, 254 },
	{ 119, 217, 194, 217, 118, 253, 207, 253 },
	{ 32, 317, 106, 315, 22, 365, 108, 363 },
	{ 119, 315, 191, 314, 118, 362, 202, 361 }

};


int main(int argc, const char** argv)
{
	//Initialise video file locations and names
	const char* file_location = "Media/";
	const char* imageFile = "TrinityCampanile1.JPG";
	const char* videofile = "PostboxesWithLines.avi";
	const char* template_file = "template.JPG";
	
	//Initialise 2d array of postbox locations
	Point2f sources[NUMBER_OF_POSTBOXES][4] = {
		{Point2f(26,113), Point2f(106,113), Point2f(13, 133), Point2f(107, 134)},
		{Point2f(119, 115),Point2f(199, 115),Point2f(119, 135),Point2f(210, 136)},
		{Point2f(30, 218), Point2f(108, 218), Point2f(18, 255), Point2f(109, 254)},
		{Point2f(119, 217), Point2f(194, 217), Point2f(118, 253), Point2f(207, 253)},
		{Point2f(32, 317), Point2f(106, 315), Point2f(22, 365), Point2f(108, 363)},
		{Point2f(119, 315), Point2f(191, 314), Point2f(118, 362), Point2f(202, 361)}
	};

	

	//Get image file and load.
	Mat image, frame, template_img;
	string imgfilename(file_location), tmpfilename(file_location);
	
	imgfilename.append(imageFile);
	image = imread(imgfilename, IMREAD_COLOR);
	tmpfilename.append(template_file);
	template_img = imread(tmpfilename, IMREAD_COLOR);

	//Check that image exists
	if (image.empty()) {
		cout << "Could not read image" << endl;
		return -1;
	}

	//Get video file and load.
	VideoCapture video;
	string filename(file_location);
	filename.append(videofile);
	video.open(filename);
	
	
	
	
	int choice;
	Point point(20, 20);
	Scalar colour(0, 0, 255);
	cout << "Press 1 to play video" << endl;
	cout << "Press 2 to look at postboxes" << endl;

	//Display text on welcoming image
	putText(image, "Press 1 to play original video.", point, FONT_HERSHEY_COMPLEX, 0.4, colour);
	point.y += 40;
	putText(image, "Press 2 view assignment demonstration.", point, FONT_HERSHEY_COMPLEX, 0.4, colour);



	do {
		imshow("Welcome", image);
		choice = cv::waitKey();
		cv::destroyAllWindows();

		switch (choice) {
		case '1':
			//Display default video
			VideoDemo(video, sources);
			break;
		case '2':
			//Perform processes
			ProcessVideo(video, sources);
			break;

		case '3':
			//Try out template matching to find locations of post boxes...
			video >> frame;
			findLocations(frame, template_img);
			break;

		default:
			break;
		}
		//Reload video
		video.open(filename);
	} while (choice != 'x');
	
	
	



	return 0;

}


