#ifndef __LANDMARKDB_H__INCLUDED__
#define __LANDMARKDB_H__INCLUDED__

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Extend this landmark class to create new landmark types
class Landmark 
{
	vector<double> pos;
	int observations;
	public:
		double distanceFrom(vector<double> pos);
		Landmark(vector<double> position, int observations=0);
};

class Chunk
{
	public:
		vector<Landmark> lms;
		Chunk(vector<Landmark> lv = vector<Landmark>());
};
// Functions:
// Stores landmarks
// Add landmarks
// Chunk based storage for speed
class LandmarkDB
{
	vector<vector<Chunk> > chunks;
	public:
		Landmark getClosestLandmark(double x, double y, double z);
		void addLandmark(Landmark l);
		LandmarkDB(double chunkSize, vector<Landmark> lms = vector<Landmark>());
};

#endif
