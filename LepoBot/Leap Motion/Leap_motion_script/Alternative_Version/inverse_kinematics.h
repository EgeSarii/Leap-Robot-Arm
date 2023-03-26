#ifndef IK_H
#define IK_H

#include <iostream>
#include <cmath>

//This function converts an angle from deg to rad.
double deg2rad(double angle_deg);

//This function converts an angle from rad to deg.
double rad2deg(double angle_rad);

//This function computes the new angles for x y z using inverse kinematics.
void ik_3(double x, double y,  double z, double& theta0, double& theta1, double& theta2, double& theta3);
#endif
