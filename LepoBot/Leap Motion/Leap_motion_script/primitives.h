#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cstdlib>
#include <string.h>
#include "Leap.h"
#include "inverse_kinematics.h"

/*
    #############
    # CONSTANTS #
    #############
*/

#define BUFSIZE 7

/*
    ############################
    # GENERAL HELPER FUNCTIONS #
    ############################
*/

//This function returns true iff serial is opened successfully. 
bool open_serial ();

//This function converts string 'message' to char* and writes its
//content to serial.
void write_to_serial (std::string message);

//This function manipulates the given Frame 'frame' and the buffers to
//determine the new angles for the robot-arm.
void update_hands_position(const Leap::Frame &frame);

//This function sets up parameters for optimizing energy consumption.
void set_up_configuration(Leap::Controller & controller);
#endif
