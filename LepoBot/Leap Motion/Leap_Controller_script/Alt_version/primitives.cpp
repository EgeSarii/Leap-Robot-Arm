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
#include "datatypes.h"
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

extern std::string  SERIAL_ID;
extern int          serial;
extern Leap::Vector position_buffer[BUFSIZE];
extern Leap::Vector EMPTY[BUFSIZE];
extern unsigned int buffer_index;
extern std::mutex   buffer_mutex;
extern std::mutex   serial_mutex;



double theta0 = 0;
double theta1 = 0;
double theta2 = 0;
double theta3 = 0;
         
         
//This function returns true if a Leap::vector contains all 0s; it is
//initialized but not modified.
bool is_empty_vec(Vector v)
{
  return v.x == 0 && v.y == 0 && v.z == 0;
}

//This function copies an array of vectors in another array.
void copy_vec (const Vector original [], Vector copy [])
{
  for (unsigned i = 0; i < BUFSIZE; i++)
    copy[i] = original[i];
}

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

void update_hands_position(const Frame &frame)
{
   int hands_amount = frame.hands().count();
   HandList hands = frame.hands();
   std::string message = "";
	
   switch (hands_amount)
   {
     case 0: return;
     case 2: return;
      
     case 1:
     {
       Hand hand = hands[0];

       buffer_mutex.lock();

       if ( buffer_index < BUFSIZE ) { position_buffer[buffer_index++] = hand.palmPosition(); }
     
       else if (buffer_index == BUFSIZE)
       {
         Vector new_position = position_buffer[BUFSIZE-1];
	 //std::cout << new_position << std::endl;

	 
	 double px = (-new_position.z +420)/10;
	 double py = (new_position.x+250)/10;
	 double pz = (new_position.y -50)/10;
	 
	 
	 std::cout <<"X: "<< px <<" Y: "<< py << " Z:" << pz<< std::endl;
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

	 message = std::to_string(theta0) + " " + std::to_string(theta1) + " " +  std::to_string(theta2) + " " + std::to_string(theta3) + " " + rotation + " " + grabber + "\r";
	 
	 std::cout << message << "\n"<< std::endl;
	 if (theta2 != 0.000000) {write_to_serial(message);}
	  
	 copy_vec (EMPTY, position_buffer);
	 buffer_index = 0;
       }
       else {std::cout << "BUFFER SIZE OVERFLOW!\n";}
     
       buffer_mutex.unlock();

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
