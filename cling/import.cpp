#include <opencv2/core.hpp>
#include <vector>
#include <iostream>

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

void goodKeys(vector<cv::KeyPoint> kp1, vector<cv::KeyPoint> kp2, vector<cv::Point2f> *inout1, vector<cv::Point2f> *inout2, vector<cv::DMatch> goodmatches)
{
	for (int i=0; i < int(goodmatches.size()); i++)
	{
		inout1->push_back(kp1[goodmatches[i].queryIdx].pt);
		inout2->push_back(kp2[goodmatches[i].trainIdx].pt);

	return change;
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
	cv::Mat out = cv::Mat(in);
	vector<cv::DMatch> goodmatches = filterMatches(descriptor1, matches);
	cv::drawMatches(in, kp1, prev, kp2, goodmatches, out, cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	return out;
}

cv::VideoCapture cap;
cv::Ptr<cv::Feature2D> surf = cv::xfeatures2d::SURF::create(400);
