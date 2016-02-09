#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace std;

vector<cv::DMatch> filterMatches(cv::Mat d1, vector<cv::DMatch> matches)
{
	double maxdist = 0; double mindist = 100;
	vector<cv::DMatch> retval;
	for (int i=0; i<d1.rows;i++)
	{
		double dist = matches[i].distance;
		if(maxdist < dist) maxdist = dist;
		if(mindist > dist) mindist = dist;
	}
	for (int i=0; i<d1.rows; i++)
	{
		if(matches[i].distance <= max(2*mindist, 0.02))
		{
			retval.push_back(matches[i]);
		}
	}
	return retval;
}

void extractPoints(vector<cv::KeyPoint> kp1, vector<cv::KeyPoint> kp2, vector<cv::Point2f> *inout1, vector<cv::Point2f> *inout2, vector<cv::DMatch> goodmatches)
{
	for (int i=0; i < int(goodmatches.size()); i++)
	{
		inout1->push_back(kp1[goodmatches[i].queryIdx].pt);
		inout2->push_back(kp2[goodmatches[i].trainIdx].pt);
	}
}

cv::Point2f avgChange(vector<cv::Point2f> pts1, vector<cv::Point2f> pts2)
{
	assert(pts1.size() == pts2.size());
	cv::Point2f accum(0.0, 0.0);
	for (int i=0; i<int(pts1.size()); i++)
	{
		accum += (pts2[i] - pts1[i]);
	}
	return accum / float(pts1.size());
}

cv::Mat process(cv::Ptr<cv::Feature2D> detector, cv::Mat in, cv::Mat prev)
{
	cv::FlannBasedMatcher matcher;
	vector<cv::KeyPoint> kp1, kp2;
	cv::Mat descriptor1, descriptor2;
	vector<cv::DMatch> matches;

	detector->detectAndCompute(in, cv::Mat(), kp1, descriptor1);
	detector->detectAndCompute(prev, cv::Mat(), kp2, descriptor2);

	matcher.match(descriptor1, descriptor2, matches);
	vector<cv::DMatch> goodmatches = filterMatches(descriptor1, matches);

	vector<cv::Point2f> sigp1, sigp2;
	extractPoints(kp1, kp2, &sigp1, &sigp2, goodmatches);
	auto avg = avgChange(sigp1, sigp2);
	if(sqrt(avg.dot(avg)) > 40.0)
	{
		//Sig point
		cout << "New sig points" << endl;
	}

	assert(goodmatches.size() > 0);

	cv::Mat out = cv::Mat(in);
	cv::drawMatches(in, kp1, prev, kp2, goodmatches, out, cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	return out;
}

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
	cv::Ptr<cv::Feature2D> surf = cv::xfeatures2d::SURF::create(400);
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

