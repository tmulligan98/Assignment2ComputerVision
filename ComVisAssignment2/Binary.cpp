#include "utilities.h"

//Mat original_image, binary_image;
//

//Otsu thresholding: Looks at all possible threshold values across the entire image. Picks a threshold...
//value where the sum of the foreground spread is similar to sum of background spread
//(spread of histogram values)

Mat otsuThresholding(Mat &frame) {
	int cur_thres, max_thres;

	cur_thres = 128;
	max_thres = 255;

	Mat gray_image;
	cvtColor(frame, gray_image, COLOR_BGR2GRAY);

	//Perform otsu thresholding
	Mat otsu_image, otsu_display_image;
	threshold(gray_image, otsu_image, cur_thres, max_thres, THRESH_BINARY | THRESH_OTSU);
	cvtColor(otsu_image, otsu_display_image, COLOR_GRAY2BGR);

	return otsu_display_image;
}

