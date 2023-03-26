#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <mutex>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>   // Contains file controls like O_RDWR.
#include <errno.h>   // Error integer and strerror() function.
#include <termios.h> // Contains POSIX terminal control definitions.

#include "Leap.h"
#include "SampleListener.h"
#include "primitives.h"
#include "inverse_kinematics.h"

using namespace Leap;

/*
  ######################
  ## HELPER FUNCTIONS ##
  ######################
This file contains the actual implementation of the functions in
"primitives.h".
Those are helper functions which will be used to obtain and process
frames received from the Leap Motion.
*/

const std::string SERIAL_ID = "/dev/ttyACM0";

extern int          serial;
extern Leap::Vector position_buffer[BUFSIZE];
extern Leap::Vector EMPTY[BUFSIZE];
extern unsigned int buffer_index;
extern std::mutex   buffer_mutex;
extern std::mutex   serial_mutex;
extern std::mutex   coordinates_mutex;

double updateX, updateY, updateZ;
double theta0 = 0;
double theta1 = 0;
double theta2 = 0;
double theta3 = 0;

//This function returns true iff serial is opened successfully. 
bool open_serial ()
{
  const char* msg = SERIAL_ID.c_str();
  serial = open( msg, O_RDWR);
    
  if (serial < 0)
  {
    std::cout << "Error " << serial << " in opening serial '"
              << SERIAL_ID << "'" << std::endl;
    return false;
   }
  return true;
}

//This function converts string 'message' to char* and writes its
//content to serial.
void write_to_serial (std::string message)
{
  char* msg = (char*) message.c_str();
  
  serial_mutex.lock();
  write(serial, msg, strlen(msg));
  serial_mutex.unlock();
  sleep(1);

  return;
}

//This function manipulates the given Frame 'frame' and the buffers to
//determine the new angles for the robot-arm.
void update_hands_position(const Frame &frame)
{
   int hands_amount = frame.hands().count();
   HandList hands = frame.hands();
   std::string message = "";
	
   switch (hands_amount)
   {
     case 0: return;
     case 2: return;
      
     case 1: //Only one hand is used to operate robot. 
     {
       Hand hand = hands[0];

       buffer_mutex.lock();

       //Circular buffer is filled.
       if ( buffer_index < BUFSIZE ) { position_buffer[buffer_index++] = hand.palmPosition(); }

       //When full, last element of buffer is used for computation.
       else if (buffer_index == BUFSIZE)
       {
         Vector new_position = position_buffer[BUFSIZE-1];
	 std::cout << new_position << std::endl;

	 //Creating virtual plane to increase movements little by little.
         coordinates_mutex.lock();
	 updateX = new_position.x;
	 if (updateX > 50) {updateX += 1;}
	 else if (updateX < -10)  { updateX -= 1;}
	 
	 updateY = new_position.y;
	 if (updateY > 180) {updateY += 1;}
	 else if (updateY < 120)  { updateY -= 1;}
	 
	 updateZ = new_position.z;
	 if (updateZ > 50) {updateZ -= 1;}
	 else if (updateZ < -20)  { updateZ += 1;}
	 	 
	 double px = (- updateZ + 420) / 10;
	 double py = (updateX   + 250) / 10;
	 double pz = (updateY   -  50) / 10;
	 
	 coordinates_mutex.unlock();
	 
	 ik_3(px, py, pz, theta0, theta1, theta2, theta3);

	 //Detect rotation.
	 double roll = hand.palmNormal().roll();
	 std::string rotation = "";
	  
         if (roll > 0.5){rotation = "pos";}
	 else if (roll < -0.5) {rotation = "neg";}
	 else {rotation = "nat";}

	 //Detect open/closure of grabber
	 std::string grabber = "still";
	 int stretched_fingers = hand.fingers().extended().count();
	 if (stretched_fingers == 5) {grabber = "open";}
	 else {grabber = "close";}

	 //Creating message to be communicated to robot over serial.
	 message = std::to_string(theta0) + " " +
	           std::to_string(theta1) + " " +
	           std::to_string(theta2) + " " +
	           std::to_string(theta3) + " " +
	           rotation + " " + grabber + "\r";
	 
	 std::cout << message << "\n"<< std::endl;
	 if (theta2 != 0.000000) {write_to_serial(message);}

	 //Resetting index of buffer to 0.
	 buffer_index = 0;
       }
       
       else {std::cout << "BUFFER SIZE OVERFLOW!\n";}
     
       buffer_mutex.unlock(); //This allows ordered execution of
                              //commands at robot side.

       return;
     }
   }
}

//This function sets up parameters for optimizing energy consumption.
void set_up_configuration(Controller & controller){
  //Setting images off.
  std::cout << "Disabling images because not needed and required to enter in power-saver mode... ";
  controller.config().setInt32("tracking_images_mode", 0);
  std::cout << "IMAGES DISABLED.\n";
/*
  SETTING POWER-MODE CONFIGURATIONS PARAMETER:

 1. acPowerSaver := Limits the frame rate to save power, even when the
                    computer is plugged into AC power.

 2. low_resource_mode_enabled : Limits the frame rate to reduce USB
                                bandwidth.
*/

  //Parameter 1:
  std::cout << "Enabling power saver... ";
  controller.config().setBool("power_saving_adapter", true);
  std::cout << "Power saver ENABLED!\n";

  //Parameter 2:
  std::cout << "Enabling low resource mode... ";
  controller.config().setBool("low_resource_mode_enabled", true);
  std::cout << "mode ENABLED!\n";
  
  //Saving configurations.
  std::cout << "Configurations are saved.\n\n";
  controller.config().save();
}
