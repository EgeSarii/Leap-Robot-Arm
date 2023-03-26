#include <iostream>
#include <cmath>

using namespace std;

double deg2rad(double angle_deg)
{
    return angle_deg * M_PI / 180.0;
}

double rad2deg(double angle_rad)
{
    return angle_rad * 180.0 / M_PI;
}

void ik(double x, double y,  double z, double& theta1, double& theta2, double& theta3, double& theta4)
{

   
  // Length of links in cm
  double a1 = 13;     //CONSTANT
  double a2 = 14;     //CONSTANT
  double a3 = 19;     //CONSTANT 

  //Desired Position of End effector
    double px;
    double py=y;
    double pz;

    if( (x>= a1)  && x<=(a1+a2+a3)){
        px = x;
    }   
    else if(x <a1){
        px =a1;
    }
    else
    { 	
        px =(a1+a2+a3);
    }   
    
    if(z <= sqrt(((a1+a2)* (a1+a2)) -((x-a1)*(x-a1)))){
        pz = z;
    }
    else{
        pz = sqrt(((a1+a2)*(a1+a2)) -((x-a1)*(x-a1)));
    }
 	
  double phi = 10;
  phi = deg2rad(phi);

  // Equations for Inverse kinematics
  double wx = px - a3*cos(phi);
  double wy = pz - a3*sin(phi);

  double c2 = (wx*wx + wy*wy - a1*a1 - a2*a2) / (2*a1*a2);
  std::cout << "New X: "<< px<< " New Y: "<< py<< " New Z: "<< pz  << endl;
  if (c2 <= 1) {
  
    double s2_1 = sqrt(abs(1-c2*c2));
    double s2_2 = -sqrt(abs(1-c2*c2));
    double theta_3_1 = atan2(s2_1, c2);
    double theta_3_2 = atan2(s2_2, c2);
    double denominator_1 = a1*a1 + a2*a2 + 2*a1*a2*cos(theta_3_1);
    double denominator_2 = a1*a1 + a2*a2 + 2*a1*a2*cos(theta_3_2);

    double s1_1 = (wy*(a1 + a2*cos(theta_3_1)) - (a2*sin(theta_3_1)*wx)) / denominator_1;
    double s1_2 = (wy*(a1 + a2*cos(theta_3_2)) - (a2*sin(theta_3_2)*wx)) / denominator_2;

    double c1_1 = (wx*(a1 + a2*cos(theta_3_1)) + (a2*sin(theta_3_1)*wy)) / denominator_1;
    double c1_2 = (wx*(a1 + a2*cos(theta_3_2)) + (a2*sin(theta_3_2)*wy)) / denominator_2;

    double theta_2_1 = atan2(s1_1, c1_1);
    double theta_2_2 = atan2(s1_2, c1_2);

    double theta_4_1 = phi - theta_2_1 - theta_3_1;
    double theta_4_2 = phi - theta_2_2 - theta_3_2;

    if (theta_2_1 >= theta_2_2)
    {
      theta2 = 0 + rad2deg(theta_2_1);
      theta3 = 150 + rad2deg(theta_4_1) ;
      theta4 = 60 -rad2deg(theta_4_1);
    }
    else
    {
      theta2 = 0 + rad2deg(theta_2_2);
      theta3 = 150 + rad2deg(theta_3_2) ;
      theta4 = 60 -rad2deg(theta_4_2);
    }

    theta1 = (py/50 * 180);
  }
}

