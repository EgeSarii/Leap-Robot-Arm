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

void ik_3(double x, double y,  double z, double& theta0, double& theta1, double& theta2, double& theta3)
{

   
  // Length of links in cm
  double a1 = 13;     //CONSTANT
  double a2 = 14;     //CONSTANT
  double a3 = 19;     //CONSTANT 

  //Desired Position of End effector
    double px;
    double py=y;
    double pz;

    if( (x>= 21)  && x<=(a1+a2+a3)){
        px = x;
    }   
    else if(x <21){
        px =21;
    }
    else
    { 	
        px =(a1+a2+a3);
    }   
    
    if(z <= sqrt(729 -( (x-21)*(x-21) ))){
        pz = z;
    }
    else{
        pz = sqrt(729 -( (x-21)*(x-21) ));
    }
 	
  double phi = 0;
  phi = deg2rad(phi);

  // Equations for Inverse kinematics
  double wx = px - a3*cos(phi);
  double wy = pz - a3*sin(phi);

  double c2 = (wx*wx + wy*wy - a1*a1 - a2*a2) / (2*a1*a2);
  std::cout << "New X: "<< px<< " New Y: "<< py<< " New Z: "<< pz  << endl;
  if (c2 <= 1) {
  
    double s2_1 = sqrt(abs(1-c2*c2));
    double s2_2 = -sqrt(abs(1-c2*c2));
    double theta_2_1 = atan2(s2_1, c2);
    double theta_2_2 = atan2(s2_2, c2);
    double denominator_1 = a1*a1 + a2*a2 + 2*a1*a2*cos(theta_2_1);
    double denominator_2 = a1*a1 + a2*a2 + 2*a1*a2*cos(theta_2_2);

    double s1_1 = (wy*(a1 + a2*cos(theta_2_1)) - (a2*sin(theta_2_1)*wx)) / denominator_1;
    double s1_2 = (wy*(a1 + a2*cos(theta_2_2)) - (a2*sin(theta_2_2)*wx)) / denominator_2;

    double c1_1 = (wx*(a1 + a2*cos(theta_2_1)) + (a2*sin(theta_2_1)*wy)) / denominator_1;
    double c1_2 = (wx*(a1 + a2*cos(theta_2_2)) + (a2*sin(theta_2_2)*wy)) / denominator_2;

    double theta_1_1 = atan2(s1_1, c1_1);
    double theta_1_2 = atan2(s1_2, c1_2);

    double theta_3_1 = phi - theta_1_1 - theta_2_1;
    double theta_3_2 = phi - theta_1_2 - theta_2_2;

    if (theta_1_1 >= theta_1_2)
    {
      theta1 = 0 + rad2deg(theta_1_1);
      theta2 = 150 + rad2deg(theta_2_1) ;
      theta3 = 60 -rad2deg(theta_3_1);
    }
    else
    {
      theta1 = 0 + rad2deg(theta_1_2);
      theta2 = 150 + rad2deg(theta_2_2) ;
      theta3 = 60 -rad2deg(theta_3_2);
    }

    theta0 = (py/50 * 180);
    //std::cout << "servo1.set_angle(" << theta0<< ")" << endl;
    //std::cout << "servo2.set_angle(0+" << theta1 << ")" << endl;
    //std::cout << "servo3.set_angle(160+" << theta2 << ")" << endl;
    //std::cout << "servo4.set_angle(135-" << theta3 << ")" << endl;
  }
  
  
}
