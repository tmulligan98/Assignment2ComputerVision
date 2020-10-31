#include "utilities.h"
#include "Video.h"


void VideoDemo(VideoCapture& default_video) {

	//Setup frame and text location
	Point location(7, 13);
	Scalar colour(0, 0, 255);
	int line_step = 13;
	
	//Play default video.
	int current_frame_count = 0;

	while (1) {
	

		Mat frames;
		default_video >> frames;

		if (frames.empty()) {
			break;
		}

		putText(frames, "Unedited video.", Point2f(0, 30), FONT_HERSHEY_SIMPLEX, 0.4, colour);
		imshow("Welcome", frames);
		
		location.y += line_step * 3 / 2;
		while (current_frame_count < 100000000) {
			current_frame_count++;
		}
		current_frame_count = 0;

		int choice = waitKey(1);
		if (choice == 1) break;
	
	
	}

	default_video.release();
	cv::destroyAllWindows();
}

void perspectiveTranforms(VideoCapture& default_video, Point2f sources[]) {

	Point2f destination[4] = { Point2f(0,90), Point2f(90,90), Point2f(0,0), Point2f(90,0) };
	Mat perspective_matrix;
	Mat perspective_warped_image;

	int current_frame_count = 0;

	while (1) {


		Mat frames;
		default_video >> frames;
		if (frames.empty()) {
			break;
		}

		perspective_warped_image = Mat::zeros(90, 90, frames.type());

		perspective_matrix = getPerspectiveTransform(sources, destination);
		warpPerspective(frames, perspective_warped_image, perspective_matrix, perspective_warped_image.size());


		

		//putText(frames, "Unedited video.", Point2f(0, 30), FONT_HERSHEY_SIMPLEX, 0.4, colour);
		imshow("Top left", perspective_warped_image);

		//location.y += line_step * 3 / 2;
		while (current_frame_count < 100000000) {
			current_frame_count++;
		}
		current_frame_count = 0;

		int choice = waitKey(1);
		if (choice == 1) break;


	}

	default_video.release();
	cv::destroyAllWindows();

}