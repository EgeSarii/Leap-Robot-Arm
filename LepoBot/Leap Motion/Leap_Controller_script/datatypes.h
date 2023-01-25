#ifndef DATATYPE_H
#define DATATYPE_H

#include <cstdlib>

/*
   #######################
   ## CUSTOM DATA TYPES ##
   #######################
This header file contains constants and data-types used in our system.
*/

#define BUFSIZE 13

//Constants which brings to the .txt files.
# define PATH "./Files/"

//Enumeration of possible Gestures to be detected.
enum CustomGesture {STILL=0, UP, DOWN, SWIPE_LEFT, SWIPE_RIGHT, GRAB, RELEASE };

//Enumeration of servos in robot-arm;
enum Servo { NONE=0, BASE, LOWER_ARM, MIDDLE_ARM, HIGHER_ARM, ROTOR, GRABBER };

//Data structure used to return a combination of Gesture for a
//specific servo of the Raspberry Pi. This is used to always have a
//pair (gesture, movement) to be able to uniquely move a servo with a
//specific movement.
struct Movement {
  CustomGesture gesture;
  Servo servo;
};
#endif
