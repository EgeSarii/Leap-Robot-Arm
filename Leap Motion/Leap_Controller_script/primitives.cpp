#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <mutex>
#include <string.h>
#include <fcntl.h>   // Contains file controls like O_RDWR.
#include <errno.h>   // Error integer and strerror() function.
#include <termios.h> // Contains POSIX terminal control definitions.

#include "Leap.h"
#include "SampleListener.h"
#include "primitives.h"
#include "datatypes.h"

using namespace Leap;

extern std::string SERIAL_ID;
extern int serial;
extern unsigned int rightHandPosIndex;
extern unsigned int left_hand_index;
extern Movement left_hand_buffer[];
extern Vector rightHandPositions[];
extern Vector EMPTY[];
extern std::mutex r_buffer_mutex;
extern std::mutex serial_mutex;

/*
    ############################
    # GENERAL HELPER FUNCTIONS #
    ############################
*/

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

//This function returns the string 'message' according to
//'movement'. This message will be written on serial.
std::string show_movement (Movement move)
{
  std::string message;
  
  switch(move.servo)
  {
    case 1:  {message = "BASE ";       break;}
    case 2:  {message = "LOWER_ARM ";  break;}
    case 3:  {message = "MIDDLE_ARM "; break;}
    case 4:  {message = "HIGHER_ARM "; break;}
    case 5:  {message = "ROTOR ";      break;}
    case 6:  {message = "GRABBER ";    break;}
    default:  message = "";
  }
    
  switch (move.gesture)
  {
    case 1:  {message += "UP ";          break;}
    case 2:  {message += "DOWN ";        break;}
    case 3:  {message += "SWIPE_LEFT ";  break;}
    case 4:  {message += "SWIPE_RIGHT "; break;}
    case 5:  {message += "GRAB ";        break;}
    case 6:  {message += "RELEASE ";     break;}
    default:  message += "";
  }

  return message;
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

  return;
}

//This function checks if the current vector has positions of the
//y-axis which in/de-creases. In this case the movement UP/DOWN is detected.
bool check_y_direction(bool dir)
{  
  for (unsigned i = 0; i < BUFSIZE-1; i++){
    
     float y1 = std::abs(rightHandPositions[i].y);
     float y2 = std::abs(rightHandPositions[i+1].y);
     const float OFFSET = 2.00;

     if ( dir ){ //True stays for UP
       if (y1  > y2 + OFFSET ||  std::abs(y1 - y2)  < OFFSET )
         return false;
    
     }else{ //False stays for DOWN
        if (y1  < y2 + OFFSET ||  std::abs(y1 - y2)  < OFFSET )
           return false;
     }
  }
  
  return true;
}

//This function checks if the current vector has positions of the
//x-axis which in/de-creases. In this case the movement
//SWIPE_RIGHT/LEFT is detected.
bool check_x_direction(bool dir)
{ 
  for (unsigned i = 0; i < BUFSIZE-1; i++){
    
     float x1 = std::abs(rightHandPositions[i].x);
     float x2 = std::abs(rightHandPositions[i+1].x);
     const float OFFSET = 0.4;

     if ( dir ){ //True stays for SWIPE_RIGHT
       if (x1  > x2 + OFFSET || std::abs(x1 - x2)  < OFFSET )
         return false;
    
     }else{ //False stays for SWIPE_LEFT
       if (x1  < x2 + OFFSET || std::abs(x1 - x2)  < OFFSET )
           return false;
     }
  }
  
  return true;
}

//This function is used to detect and classify custom gestures of the
//right hand.
CustomGesture detect_right_gesture(Hand hand, int fingers )
{
   //RIGHT HAND IS USED FOR MOVEMENTS OF ARMS OF ROBOT.
  CustomGesture gest = STILL;
  if (hand.isRight() && !is_empty_vec(rightHandPositions[BUFSIZE-1]))
  {
    if (fingers == 0){ //No movements are sent; possible to reposition hand.
      gest =  STILL;
    }    
    else if (fingers == 2 || fingers == 3 || fingers == 4) //When the specified amount of fingers is used,
    {                                                      //it is possible to move the related robot-arms up and down.  
      if (check_y_direction(true)) {
	gest =  UP;
      }

      else if (check_y_direction(false)) {
	gest =  DOWN;
      }
    }    
    else { //This considers cases with 1 or 5 stretched fingers.
      if (check_x_direction(true)){
	gest = SWIPE_RIGHT;
      }
      else if (check_x_direction (false)){
	gest =  SWIPE_LEFT;
      }
    }

  //Needed to empty global vector and store new frames.
  //This prevent circular buffer to consider previous data.
  rightHandPosIndex = 0;
  copy_vec (EMPTY, rightHandPositions);
  }
 
  return gest;
}

//This function detects movement of the left hand: it checks if
//movements GRAB and RELEASE are set.
CustomGesture detect_left_gesture(Hand hand, int fingers )
{ //LEFT HAND IS USED FOR GRIPPING OR RELEASING THE GRABBER ONLY.
  if (hand.isLeft())
  {
    sleep(0.85);
    switch (fingers)
    {
      case 5:  return RELEASE;
      case 0:  return GRAB;
    }
  }
  return STILL;
}

//This function uses the amount of fingers and the gesture to assign
//the correct servo of the right hand.
Servo detect_right_servo (int fingers, CustomGesture gesture)
{
  if ( gesture == SWIPE_LEFT || gesture == SWIPE_RIGHT )
  {
    if (fingers == 5){return BASE;}
    else if (fingers == 1) {return ROTOR;}
  }
  switch (fingers)
  {
    case 2 : return LOWER_ARM;
    case 3 : return MIDDLE_ARM;
    case 4 : return HIGHER_ARM;
    default: return NONE;
  }
}

//This function starts processing data of the right hand.
//At the end of execution, the movement of the right hand is detected.
Movement handle_right (Hand hand)
{
     Hand right = hand;
     FingerList r_fingers = right.fingers();
     int stretched_fingers = r_fingers.extended().count();
     Movement current_mov = {STILL, NONE};

     //Detect movement
     r_buffer_mutex.lock();

     if ( rightHandPosIndex < BUFSIZE ){rightHandPositions[rightHandPosIndex++] = right.palmPosition();}
     
     else if (rightHandPosIndex == BUFSIZE)
     {
       current_mov.gesture = detect_right_gesture(right, stretched_fingers);
       current_mov.servo = detect_right_servo (stretched_fingers, current_mov.gesture);
     }
     
     r_buffer_mutex.unlock();

     return current_mov;
}

//This function starts processing data of the left hand.
//At the end of execution, the movement of the left hand is detected.
Movement handle_left( Hand hand)
{
  Hand left = hand;
  FingerList l_fingers = left.fingers();
  int stretched_fingers = l_fingers.extended().count();
  Movement current_mov = {STILL, GRABBER};

  //Detect movement
  current_mov.gesture = detect_left_gesture(left, stretched_fingers);
  return current_mov;
}

//This function assigns the correct hand to hands 'left' and 'right'
//and the respective movements.
Movement assign_hands(const Hand hand)
{
  Movement cur_movement = {STILL, NONE};
  
  if (hand.isLeft())
    cur_movement = handle_left(hand);
    
  if (hand.isRight())
    cur_movement = handle_right(hand);

  return cur_movement;
}

//This function checks that the left hand buffer "left_hand_buffer" is
//full of commands.
bool left_buf_full()
{
  return left_hand_index == BUFSIZE;
}

//This function returns true iff the left hand buffer is full.
bool manage_left_buf(Movement move)
{
  if (left_buf_full())
  {
    left_hand_index = 0;
    return true;
  }
  else
  {
    left_hand_buffer[left_hand_index++] = move;
    return false;
  }    
}

//This function checks the amount of hands captured by the Leap Motion
//and writes to serial the respective movements.
//The string written to serial is parsed to instruct the 6-DOF robot.
void update_hands_position(const Frame &frame)
{
  
  int hands_amount = frame.hands().count();
  HandList hands = frame.hands();
  Movement robot_command = {STILL, NONE};
  std::string message = "";
  
  switch (hands_amount)
  {
     case 0: //This should be never met by use of inner class SimpleListener.
       return;
       
     case 1:
     {
        robot_command = assign_hands(hands[0]);
        message = show_movement(robot_command)+ "\r";
	//	std::cout << message;

	if ((hands[0].isLeft() && manage_left_buf(robot_command)) || hands[0].isRight())
	{
	  write_to_serial(message);
	  std::cout << message;
	}
	    
        std::cout << std::endl;
        break;
     }

     case 2:
     {
       robot_command = assign_hands(hands[0]);
       if ( (hands[0].isLeft() && manage_left_buf(robot_command)) || hands[0].isRight())
       {
	 message = show_movement(robot_command);
       }
       
       robot_command = assign_hands(hands[1]);
       if ( (hands[1].isLeft() && manage_left_buf(robot_command)) || hands[1].isRight())
       {
	 message += show_movement(robot_command);
       }
       
       message += "\r";
       write_to_serial(message);
	
       std::cout << message;
       std::cout << std::endl;
       
       break;
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
