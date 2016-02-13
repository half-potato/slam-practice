#ifndef __LANDMARKDB_C__INCLUDED__
#define __LANDMARKDB_C__INCLUDED__

#include "LandmarkDB.h"
#include <array>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <math.h>

using namespace std;

Landmark::Landmark(vector<double> position, int observations) : pos(position), observations(observations) {}

// Point landmark implementation
double Landmark::distanceFrom(vector<double> pos)
{
	vector<double> diff;
	set_difference(pos.begin(), pos.end(), this->pos.begin(), this->pos.end(), back_inserter(diff));
	return sqrt(inner_product(diff.begin(), diff.end(), diff.begin(), double(0.0)));
}

// LandmarkDB implementation
void LandmarkDB::addLandmark(Landmark l)
{
	int cx = floor(l.pos[0]/this->chunkSize);
	int cy = floor(l.pos[1]/this->chunkSize);
	this->chunks[cx][cy].push_back(l);
}

Landmark *LandmarkDB::getClosestLandmark(vector<double> pos)
{
	int cx = floor(pos[0]/this->chunkSize);
	int cy = floor(pos[1]/this->chunkSize);
	vector<Landmark> chunk = this->chunks[cx][cy];
	int leastIndex = 0;
	vector<double> diff;
	set_difference(chunk[leastIndex].pos.begin(), chunk[leastIndex].pos.end(), pos.begin(), pos.end(), back_inserter(diff));
	double least = inner_product(diff.begin(), diff.end(), diff.begin(), double(0.0));
	for (int i=0; i<int(chunk.size()); i++)
	{
		set_difference(chunk[leastIndex].pos.begin(), chunk[leastIndex].pos.end(), pos.begin(), pos.end(), back_inserter(diff));
		double dist = inner_product(diff.begin(), diff.end(), diff.begin(), double(0.0));
		if (least > dist)
		{
			least = dist;
			leastIndex = i;
		}
	}
	return &this->chunks[cx][cy][leastIndex];
}

LandmarkDB::LandmarkDB(double chunkSize, vector<Landmark> lms) :chunkSize(chunkSize) 
{
	for (int i=0; i<int(lms.size()); i++)
	{
		addLandmark(lms[i]);
	}
}
#endif
