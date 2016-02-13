#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include "filter.cpp"

using namespace std;

int main()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) 
	{
		cout << "Cannot open the video camera" << endl;
		return -1;
	}
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Opening video capture with size " << dWidth << ", " << dHeight << endl;

	cv::namedWindow("V", CV_WINDOW_AUTOSIZE);

	int sliderVal1 = 50;
	cv::createTrackbar("Brightness", "V", &sliderVal1, 100);
	int sliderVal2 = 50;
	cv::createTrackbar("Contrast", "V", &sliderVal2, 100);

	//Mat img(100, 300, CV_8UC3, Scalar(0, 0, 100));
	auto surf = cv::xfeatures2d::SURF::create(400, 2, 3);
	cv::Mat prevImg;

	while(1)
	{
		cv::Mat frame;
		bool success = cap.read(frame);
		if (!success)
		{
			cout << "Cannot read frame from camera" << endl;
			break;
		}
		cv::Mat adj;
		frame.convertTo(adj, -1, double(sliderVal2) / 50.0, sliderVal1 - 50);
		cv::Mat gray;
		cv::cvtColor(adj, gray, CV_BGR2GRAY);
		//gray.convertTo(gray, CV_8U, 0.00390625);
		if(prevImg.empty())
		{
			prevImg = cv::Mat(gray);
		}
		cv::Mat visible = process(surf, gray, prevImg);
		if (visible.empty())
		{
			imshow("V", gray);
		} else {
			imshow("V", visible);
		}
		if(cv::waitKey(30) == 27)
		{
			cout << "Done" << endl;
			break;
		}
		prevImg = frame;
	}
	return 0;
}

