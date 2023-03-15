from numpy import *



def ik_3(x, z):
  # Length of links in cm
  a1= 13
  a2 = 14
  a3 = 19

  # Desired Position of End effector
  px = x
  py = z

  phi = 0
  phi = deg2rad(phi)

  # Equations for Inverse kinematics
  wx = px - a3*cos(phi)
  wy = py - a3*sin(phi)

  c2 = (wx**2 + wy**2 - a1**2 - a2**2)/ (2*a1*a2)
  if c2 <= 1:
        
    s2_1 = sqrt(abs(1-c2**2))
    s2_2 = - sqrt(abs(1- c2**2))
  
    theta_2_1 = arctan2(s2_1, c2)
    theta_2_2 = arctan2(s2_2, c2)
    denominator_1 = a1**2 + a2**2 + 2*a1*a2*cos(theta_2_1)
    denominator_2 = a1**2 + a2**2 + 2*a1*a2*cos(theta_2_2)

    s1_1 = (wy*(a1 + a2*cos(theta_2_1)) - (a2*sin(theta_2_1)*wx))/(denominator_1)
    s1_2 = (wy*(a1 + a2*cos(theta_2_2)) - (a2*sin(theta_2_2)*wx))/(denominator_2)

    c1_1 = (wx*(a1 + a2*cos(theta_2_1)) + (a2*sin(theta_2_1)*wy))/(denominator_1)
    c1_2 = (wx*(a1 + a2*cos(theta_2_2)) + (a2*sin(theta_2_2)*wy))/(denominator_2)

    theta_1_1 = arctan2(s1_1,c1_1)
    theta_1_2 = arctan2(s1_2,c1_2)
    
  
    theta_3_1 = phi-theta_1_1 - theta_2_1
    theta_3_2 = phi-theta_1_2 - theta_2_2

    if(theta_1_1 >=theta_1_2):
          theta1 = rad2deg(theta_1_1)
          theta2 = rad2deg(theta_2_1)
          theta3 = rad2deg(theta_3_1)


    else:
          theta1 = rad2deg(theta_1_2)
          theta2 = rad2deg(theta_2_2)
          theta3 = rad2deg(theta_3_2)

    angles= [theta1, theta2, theta3]
  #print('x: {}, z: {}'.format(px, py))
  
  #print('theta_1: ', rad2deg(theta1))
  #print('theta_2: ', rad2deg(theta2))
  #print('theta_3: ', rad2deg(theta3))

  
  print(angles)
  return(angles)


ik_3(21, 2)