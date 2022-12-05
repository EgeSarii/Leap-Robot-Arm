#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cstdlib>
#include <string.h>
#include "Leap.h"
#include "datatypes.h"

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


//This function returns the string 'message' according to
//'movement'. This message will be written on serial.
std::string show_movement (Movement move);


//This function returns true iff serial is opened successfully. 
bool open_serial ();


//This function converts string 'message' to char* and writes its
//content to serial.
void write_to_serial (std::string message);


//This function checks if the current vector has positions of the
//y-axis which in/de-creases. In this case the movement UP/DOWN is detected.
bool check_y_direction(bool dir);


//This function checks if the current vector has positions of the
//x-axis which in/de-creases. In this case the movement
//SWIPE_RIGHT/LEFT is detected.
bool check_x_direction(bool dir);


//This function is used to detect and classify custom gestures of the
//right hand.
CustomGesture detect_right_gesture(Leap::Hand hand, int fingers );


//This function detects movement of the left hand: it checks if
//movements GRAB and RELEASE are set.
CustomGesture detect_left_gesture(Leap::Hand hand, int fingers );


//This function detects a clapping movement.
bool detect_clap(Leap::Hand hand_1, Leap::Hand hand_2);


//This function uses the amount of fingers and the gesture to assign
//the correct servo of the right hand.
Servo detect_right_servo (int fingers, CustomGesture gesture);


//This function starts processing data of the right hand.
//At the end of execution, the movement of the right hand is detected.
Movement handle_right (Leap::Hand hand);


//This function starts processing data of the left hand.
//At the end of execution, the movement of the left hand is detected.
Movement handle_left(Leap::Hand hand);


//This function assigns the correct hand to hands 'left' and 'right'
//and the respective movements.
Movement assign_hands(const Leap::Hand hand);


//This function checks that the left hand buffer "left_hand_buffer" is
//full of commands.
bool left_buf_full();

//This function returns true iff the left hand buffer is full.
//In this case the index of such buffer is set to 0.
//Otherwise the commands is added to the buffer.
bool manage_left_buf(Movement move);

//This function checks the amount of hands captured by the Leap Motion
//and writes to serial the respective movements.
//The string written to serial is parsed to instruct the 6-DOF robot.
void update_hands_position(const Leap::Frame &frame);


//This function sets up parameters for optimizing energy consumption.
void set_up_configuration(Leap::Controller & controller);
#endif
