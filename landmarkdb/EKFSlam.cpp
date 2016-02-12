#ifndef __EKFSLAM_C__INCLUDED__
#define __EKFSLAM_C__INCLUDED__

#include "EKFSlam.h"

Landmark updateLandmark(Landmark l);
void updateLandmarks(double *laserData, double *bounds);
void ekfUpdateLandmarks(bool *matched, int* id, double *ranges, double *bearings, double *robotPosition);

#endif
