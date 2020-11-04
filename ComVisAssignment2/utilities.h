#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <iostream>
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;


//Video functions
void VideoDemo(VideoCapture& default_video);
void perspectiveTranforms(VideoCapture& default_video, Point2f sources[][4]);

//Binary functions
Mat otsuThresholding(Mat &frame);
Mat adaptiveThresholding(Mat &frame);

//Movement detection
bool detectMovement(Mat frame, Ptr<BackgroundSubtractor> background_model);