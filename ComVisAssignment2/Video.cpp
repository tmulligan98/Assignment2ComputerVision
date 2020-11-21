#include "utilities.h"



void VideoDemo(VideoCapture& default_video, Point2f sources[][4]) {

	//Setup frame and text location
	Point location(7, 13);
	Scalar colour(0, 0, 255);
	int line_step = 13;

	int frame_no = 1;
	
	//Play default video.
	int current_frame_count = 0;

	while (1) {
		

		Mat frames;
		default_video >> frames;


		/*if (frames.empty()) {
			break;
		}*/
		if (!default_video.read(frames))
			break;

		putText(frames, "Unedited video.", Point2f(0, 30), FONT_HERSHEY_SIMPLEX, 0.4, colour);
		imshow("Welcome", frames);
		//cout << frame_no << endl;
		
		//Below is the function call to view the post-box frame "mask"
		//createMask(frames, sources);


		
		
		while (current_frame_count < 100000000) {
			current_frame_count++;
		}
		current_frame_count = 0;

		int choice = waitKey(1);
		if (choice == 1) break;

		frame_no++;
	
	
	}

	default_video.release();
	cv::destroyAllWindows();
}

void ProcessVideo(VideoCapture& default_video, Point2f sources[][4]) {

	char c;
	int frame_count = 1;
	Mat combined_binary;

	//

	Point2f destination[4] = { Point2f(0,90), Point2f(90,90), Point2f(0,0), Point2f(90,0) };
	Mat perspective_matrices[NUMBER_BOXES];
	Mat perspective_warped_images[NUMBER_BOXES];
	//Mat perspective_binary_image[NUMBER_BOXES];
	Mat prev_frame;
	Mat combined;
	bool *post;

	

	

	//Setup frame and text location
	Point location(7, 13);
	Scalar colour(0, 0, 255);
	int line_step = 13;
	bool isMovement;

	int current_frame_count = 0;

	while (1) {


		Mat frames;
		//Take next frame
		default_video >> frames;
		//If no frames left, break out
		if (frames.empty()) {
			break;
		}

		
		//Perform perspective transformation on each postbox
		perspectiveTransforms(frames, perspective_matrices, perspective_warped_images, sources, destination);

		
		//Combine the perspective frames in to one larger picture. Useful to actually see what's going on.
		combined = RecreateFrame(perspective_warped_images);

		//Perform motion detection on entire frame
		isMovement = detectMovement_FrameDifference(frames, prev_frame);

		//If no movement, look for edges in boxes. This is where we figure out if we actually have post or not
		if (!isMovement) {

			//Check if frame of boxes is visible...
			post = computeEdges(perspective_warped_images, combined_binary);

			if (!isEmpty(post)) {
				cout << frame_count << ", Post in, ";
				if (post[0]) {
					putText(combined, "Post", Point2f(0, 20), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					putText(frames, "Post", Point2f(0, 20), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "1 ";
				}	//Top left...
				if (post[1]) {
					putText(combined, "Post", Point2f(90, 10), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					putText(frames, "Post", Point2f(90, 10), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "2 ";
				}
				if (post[2]) {
					putText(combined, "Post", Point2f(0, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					putText(frames, "Post", Point2f(0, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "3 ";
				}
				if (post[3]) {
					putText(combined, "Post", Point2f(90, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					putText(frames, "Post", Point2f(90, 180), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "4 ";
				}
				if (post[4]) {
					putText(combined, "Post", Point2f(0, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					putText(frames, "Post", Point2f(0, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "5 ";
				}
				if (post[5]) {
					putText(combined, "Post", Point2f(90, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					putText(frames, "Post", Point2f(90, 270), FONT_HERSHEY_SIMPLEX, 0.4, colour);
					cout << "6 ";
				}	//Bottom right...
			}
			else  cout << frame_count << ", " << "No Post"; 
			
			cout << endl;

			*post = {};
		
			
		}
		else {
			//If there's movement, say so.
			putText(combined, "Movement!!!!", Point2f(0, 30), FONT_HERSHEY_DUPLEX, 0.6, colour);
			putText(frames, "Movement!!!!", Point2f(0, 30), FONT_HERSHEY_DUPLEX, 0.6, colour);
			cout << frame_count << ", " << "View Obscured" << endl;
		}
		
		putText(frames, to_string(frame_count), Point2f(0, 10), FONT_HERSHEY_DUPLEX, 0.6, colour);
		putText(combined, to_string(frame_count), Point2f(0, 10), FONT_HERSHEY_DUPLEX, 0.6, colour);
		//imshow("Perspectives combined", combined);
		//imshow("Default", frames);
		
		
		
		imshow("Combined", combined);
		imshow("Full Scene", frames);
		imshow("Binary perspectives", combined_binary);

		//

		//Set delay.
		while (current_frame_count < 100000000) 
			current_frame_count++;
		
		current_frame_count = 0;

		//Option to break out
		int choice = waitKey(1);
		if (choice == 1) break;

		frame_count++;
		
	}

	default_video.release();
	cv::destroyAllWindows();

}
