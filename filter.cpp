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
	if(sqrt(avg.dot(avg)) > 10.0)
	{
		//Sig point
		cout << "New sig points" << endl;
	}

	assert(goodmatches.size() > 0);

	cv::Mat out = cv::Mat(in);
	cv::drawMatches(in, kp1, prev, kp2, goodmatches, out, cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	return out;
}
