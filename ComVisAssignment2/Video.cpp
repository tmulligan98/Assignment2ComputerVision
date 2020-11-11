#include "utilities.h"



void VideoDemo(VideoCapture& default_video, Point2f sources[][4]) {

	//Setup frame and text location
	Point location(7, 13);
	Scalar colour(0, 0, 255);
	int line_step = 13;
	
	//Play default video.
	int current_frame_count = 0;

	while (1) {
	

		Mat frames;
		default_video >> frames;

		//frames = otsuThresholding(frames);

		if (frames.empty()) {
			break;
		}

		putText(frames, "Unedited video.", Point2f(0, 30), FONT_HERSHEY_SIMPLEX, 0.4, colour);
		imshow("Welcome", frames);
		
		//Below is the function call to view the post-box frame "mask"
		//createMask(frames, sources);


		
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

void perspectiveTranforms(VideoCapture& default_video, Point2f sources[][4]) {

	Point2f destination[4] = { Point2f(0,90), Point2f(90,90), Point2f(0,0), Point2f(90,0) };
	Mat perspective_matrices[6];
	Mat perspective_warped_images[6];
	Mat perspective_binary_image[6];
	Mat prev_frame;
	Mat combined;
	int frame_count = 1;

	bool *post;

	//Declare Gaussian background model
	//Ptr<BackgroundSubtractor> background;
	//background = createBackgroundSubtractorMOG2();

	//Setup frame and text location
	Point location(7, 13);
	Scalar colour(0, 0, 255);
	int line_step = 13;
	bool isMovement;

	int current_frame_count = 0;

	while (1) {


		Mat frames;
		default_video >> frames;
		if (frames.empty()) {
			break;
		}

		//Perform perspective transformation on each postbox

		perspective_warped_images[0] = Mat::zeros(90, 90, frames.type());
		perspective_matrices[0] = getPerspectiveTransform(sources[0], destination);
		warpPerspective(frames, perspective_warped_images[0], perspective_matrices[0], perspective_warped_images[0].size());

		perspective_warped_images[1] = Mat::zeros(90, 90, frames.type());
		perspective_matrices[1] = getPerspectiveTransform(sources[1], destination);
		warpPerspective(frames, perspective_warped_images[1], perspective_matrices[1], perspective_warped_images[1].size());

		perspective_warped_images[2] = Mat::zeros(90, 90, frames.type());
		perspective_matrices[2] = getPerspectiveTransform(sources[2], destination);
		warpPerspective(frames, perspective_warped_images[2], perspective_matrices[2], perspective_warped_images[2].size());

		perspective_warped_images[3] = Mat::zeros(90, 90, frames.type());
		perspective_matrices[3] = getPerspectiveTransform(sources[3], destination);
		warpPerspective(frames, perspective_warped_images[3], perspective_matrices[3], perspective_warped_images[3].size());

		perspective_warped_images[4] = Mat::zeros(90, 90, frames.type());
		perspective_matrices[4] = getPerspectiveTransform(sources[4], destination);
		warpPerspective(frames, perspective_warped_images[4], perspective_matrices[4], perspective_warped_images[4].size());

		perspective_warped_images[5] = Mat::zeros(90, 90, frames.type());
		perspective_matrices[5] = getPerspectiveTransform(sources[5], destination);
		warpPerspective(frames, perspective_warped_images[5], perspective_matrices[5], perspective_warped_images[5].size());

		
		//Combine the perspective frames in to one larger picture. Useful to actually see what's going on.
		combined = RecreateFrame(perspective_warped_images);

		//Perform motion detection on entire frame
		isMovement = detectMovement_FrameDifference(frames, prev_frame);

		//If no movement, look for edges in boxes. This is where we figure out if we actually have post or not
		if (!isMovement) {
			post = computeEdges(perspective_warped_images);
			if (!isEmpty(post)) {
				cout << frame_count << ", Post in, ";
				if (post[0]) {
					putText(combined, "Post", Point2f(0, 10), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "1 ";
				}	//Top left...
				if (post[1]) {
					putText(combined, "Post", Point2f(90, 10), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "2 ";
				}
				if (post[2]) {
					putText(combined, "Post", Point2f(0, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "3 ";
				}
				if (post[3]) {
					putText(combined, "Post", Point2f(90, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "4 ";
				}
				if (post[4]) {
					putText(combined, "Post", Point2f(0, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "5 ";
				}
				if (post[5]) {
					putText(combined, "Post", Point2f(90, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "6 ";
				}	//Bottom right...
			}
			else  cout << frame_count << ", " << "No Post"; 
			
			cout << endl;

			*post = {};
		
			
		}
		else {
			putText(combined, "Movement!!!!", Point2f(0, 30), FONT_HERSHEY_DUPLEX, 0.6, colour);
			cout << frame_count << ", " << "View Obscured" << endl;
		}
		
		imshow("Perspectives combined", combined);
		
		
		
		//imshow("Combined", combined);
		//imshow("Full Scene", frames);



		while (current_frame_count < 100000000) 
			current_frame_count++;
		
		current_frame_count = 0;

		int choice = waitKey(1);
		if (choice == 1) break;

		frame_count++;
	}

	default_video.release();
	cv::destroyAllWindows();

}