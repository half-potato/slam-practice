#ifndef __LANDMARKDB_C__INCLUDED__
#define __LANDMARKDB_C__INCLUDED__

#include "Landmarkdb.h"
#include <array>
#include <numeric>
#include <iterator>

using namespace std;

Landmark::Landmark(vector<double> position, int observations) : pos(position), observations(observations) {}

// Point landmark implementation
double Landmark::distanceFrom(vector<double> pos)
{
	vector<double> diff;
	set_difference(pos.begin(), pos.end(), this->pos.begin(), this->pos.end(), back_inserter(diff));
	return inner_product(diff.begin(), diff.end(), diff.begin(), double(0.0));
}

// Chunk implementation
Chunk::Chunk(vector<Landmark> lv) :lms(lv) {}

// LandmarkDB implementation
LandmarkDB(double chunkSize, vector<Landmark> lms = vector<Landmark>()) {

}
#endif
