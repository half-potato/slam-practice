#ifndef __EKFSLAM_H__INCLUDED__
#define __EKFSLAM_H__INCLUDED__

#include <iostream>
#include <fstream>
#include <vector>
#include "LandmarkDB.h"

using namespace std;

class EKFSlam
{
	public:
		LandmarkDB *db;
		Landmark updateLandmark(Landmark l);
		void updateLandmarks(double *laserData, double *bounds);
		void ekfUpdateLandmarks(bool *matched, int* id, double *ranges, double *bearings, double *robotPosition);
};

#endif
