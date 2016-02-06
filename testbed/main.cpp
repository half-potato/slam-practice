#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened()) 
	{
		cout << "Cannot open the video camera" << endl;
		return -1;
	}
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Opening video capture with size " << dWidth << ", " << dHeight << endl;

	namedWindow("V", CV_WINDOW_AUTOSIZE);

	int sliderVal1 = 50;
	createTrackbar("Brightness", "V", &sliderVal1, 100);
	int sliderVal2 = 50;
	createTrackbar("Contrast", "V", &sliderVal2, 100);

	//Mat img(100, 300, CV_8UC3, Scalar(0, 0, 100));
	while(1)
	{
		Mat frame;
		bool success = cap.read(frame);
		if (!success)
		{
			cout << "Cannot read frame from camera" << endl;
			break;
		}
		Mat adj;
		frame.convertTo(adj, -1, double(sliderVal2) / 50.0, sliderVal1 - 50);
		imshow("V", adj);
		if(waitKey(30) == 27)
		{
			cout << "Done" << endl;
			break;
		}
	}
	return 0;
}
