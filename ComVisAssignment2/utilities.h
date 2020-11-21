#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <iostream>
#define PI 3.14159265358979323846
#define NUMBER_BOXES 6
#define MAX_FRAMES 95

using namespace std;
using namespace cv;

//Useful declarations

//Video functions
void VideoDemo(VideoCapture& default_video, Point2f sources[][4]);
void ProcessVideo(VideoCapture& default_video, Point2f sources[][4]);

//Binary functions
Mat otsuThresholding(Mat &frame);
//Mat adaptiveThresholding(Mat &frame);

//Movement detection
bool detectMovementGMM(Mat frame, Ptr<BackgroundSubtractor> background_model);
bool detectMovement_FrameDifference(Mat cur_frame, Mat &prev_frame);

//Edges detection
bool* computeEdges(Mat images[NUMBER_BOXES], Mat &combined_binary);

//Useful Functions
Mat RecreateFrame(Mat perspectives[NUMBER_BOXES]);
bool closeEnough(int a, int b, int c, int d);
void createMask(Mat frame, Point2f sources[][4]);
bool isEmpty(bool arr[NUMBER_BOXES]);
void findLocations(Mat frame, Mat template_image);
void equaliseShadows(Mat &frame);

void perspectiveTransforms(Mat frame, Mat perspective_matrices[], Mat perspective_warped_images[], Point2f sources[][4], Point2f destinations[]);