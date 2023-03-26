#ifndef IK_H
#define IK_H

#include <iostream>
#include <cmath>

double deg2rad(double angle_deg);

double rad2deg(double angle_rad);

void ik_3(double x, double y,  double z, double& theta0, double& theta1, double& theta2, double& theta3);
#endif
