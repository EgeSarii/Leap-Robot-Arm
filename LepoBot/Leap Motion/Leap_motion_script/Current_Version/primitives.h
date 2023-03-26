#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cstdlib>
#include <string.h>
#include "Leap.h"
#include "datatypes.h"
#include "inverse_kinematics.h"

/*
    ############################
    # GENERAL HELPER FUNCTIONS #
    ############################
*/

//This function returns true if a Leap::vector contains all 0s; it is
//initialized but not modified.
bool is_empty_vec(Leap::Vector v);


//This function copies an array of vectors in another array.
void copy_vec (const Leap::Vector original [], Leap::Vector copy []);


//This function returns true iff serial is opened successfully. 
bool open_serial ();


//This function converts string 'message' to char* and writes its
//content to serial.
void write_to_serial (std::string message);

//This function checks the amount of hands captured by the Leap Motion
//and writes to serial the respective movements.
//The string written to serial is parsed to instruct the 6-DOF robot.
void update_hands_position(const Leap::Frame &frame);


//This function sets up parameters for optimizing energy consumption.
void set_up_configuration(Leap::Controller & controller);
#endif
