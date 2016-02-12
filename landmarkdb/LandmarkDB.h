#ifndef __LANDMARKDB_H__INCLUDED__
#define __LANDMARKDB_H__INCLUDED__

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Extend this landmark class to create new landmark types
class Landmark 
{
	public:
		vector<double> pos;
		int observations;
		double distanceFrom(vector<double> pos);
		Landmark(vector<double> position, int observations=0);
};

// Functions:
// Stores landmarks
// Add landmarks
// Chunk based storage for speed
class LandmarkDB
{
	vector<vector<vector<Landmark > > > chunks;
	public:
		double chunkSize;
		Landmark *getClosestLandmark(vector<double> pos);
		void addLandmark(Landmark l);
		LandmarkDB(double chunkSize, vector<Landmark> lms = vector<Landmark>());
};

#endif
