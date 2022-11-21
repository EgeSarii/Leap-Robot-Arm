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

using namespace Leap;

/*
    #############
    # CONSTANTS #
    #############
*/

//Enumeration of possible Gestures to be detected.
enum CustomGesture { STILL = 0, UP, DOWN, SWIPE_LEFT, SWIPE_RIGHT, GRAB, RELEASE};

//Enumeration of servos in robot-arm;
enum Servo { NONE = 0, BASE, LOWER_ARM, MIDDLE_ARM, HIGHER_ARM, ROTOR, GRABBER };

//Location of serial device to be written.
const std::string SERIAL_ID = "/dev/ttyACM1";

//Size of circular buffer.
const unsigned SIZE = 10;

//EMPTY array of Leap::vectors to clean buffer.
const Vector EMPTY[SIZE] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};

/*
   #######################
   ## CUSTOM DATA TYPES ##
   #######################
*/

//Data structure used to return a combination of Gesture for a
//specific servo of the Raspberry Pi. This is used to always have a
//pair (gesture, movement) to be able to uniquely move a servo with a
//specific movement.
struct Movement {
  CustomGesture gesture;
  Servo servo;
};

/*
    ####################
    # GLOBAL VARIABLES #
    ####################
*/

//This array of vectors will store the 10 previous vectors of a
//given frame. This is used in order to compute gestures and catch
//actions. Here Vector is a Leap::Vector; not std::vector.
//This is used as a circular buffer of size SIZE.
Vector rightHandPositions[SIZE] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};

//This variable is used to keep track of the indexes of such circular buffer.
unsigned rightHandPosIndex = 0;

//Mutex needed because inner class is actually executed in different tread.
std::mutex serial_mutex; //Needed to correctly print commands to
			 //serial and std::out. Without it we had
			 //issues.

std::mutex r_buffer_mutex; //Mutex for ensure synchronization on
			   //global variable 'rightHandPositions'.

//This will obtain the process id once the function "open" is called.
int serial = 0;
/*
    ############################
    # GENERAL HELPER FUNCTIONS #
    ############################
*/

//This function returns true if a Leap::vector contains all 0s; it is
//initialized but not modified.
bool is_empty(Vector v)
{
  return v.x == 0 && v.y == 0 && v.z == 0;
}

//This function copies an array of vectors in another array.
void copy_vec (const Vector original [], Vector copy [])
{  
  for (unsigned i = 0; i < SIZE; i++)
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
    default:  message = "NONE ";
  }
    
  switch (move.gesture)
  {
    case 1:  {message += "UP ";          break;}
    case 2:  {message += "DOWN ";        break;}
    case 3:  {message += "SWIPE_LEFT ";  break;}
    case 4:  {message += "SWIPE_RIGHT "; break;}
    case 5:  {message += "GRAB ";        break;}
    case 6:  {message += "RELEASE ";     break;}
    default:  message += "STILL ";
  }
  
  return message;
}

//This function returns true iff serial is opened successfully. 
bool open_serial (int & serial)
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
  for (unsigned i = 0; i < SIZE-1; i++){
    
     float y1 = std::abs(rightHandPositions[i].y);
     float y2 = std::abs(rightHandPositions[i+1].y);
     const float OFFSET = 1.50;

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
  for (unsigned i = 0; i < SIZE-1; i++){
    
     float x1 = std::abs(rightHandPositions[i].x);
     float x2 = std::abs(rightHandPositions[i+1].x);
     const float OFFSET = 2.00;

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
  if (hand.isRight() && !is_empty(rightHandPositions[SIZE-1]))
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

     if ( rightHandPosIndex < SIZE ){rightHandPositions[rightHandPosIndex++] = right.palmPosition();}
     
     else if (rightHandPosIndex == SIZE)
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

//This function checks the amount of hands captured by the Leap Motion
//and writes to serial the respective movements.
//The string written to serial is parsed to instruct the 6-DOF robot.
void update_hands_position(const Frame &frame, int serial)
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
        std::cout << message;

        write_to_serial(message);
	
        std::cout << std::endl;
        break;
     }

     case 2:
     {
       robot_command = assign_hands(hands[0]);
       message = show_movement(robot_command);
       
       robot_command = assign_hands(hands[1]);
       message = message + (show_movement(robot_command)) + "\r";
       
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

/*
   ############################
   # INNER CLASS FOR LISTENER #
   ############################
*/

/*
   The connection process is asynchronous and hence a method to obtain
   the data from the sensor is required. In this script a "Listener"
   is used.
 
   A Listener can be added to the Controller, which provides an
   event-based mechanism for responding to important Controller state
   changes.

   Listeners are not always the best idea.
   The reason are:

	1. Listeners use idependent threads to invoke the implemented
           code for each method. When using Listeners, a thread-safe
           programming style should be used.

        2. Overhead might occur when using threads.


  As alternative to Listeners, it is possible to get the Controller
  object for frames.

In order to use Listeners, the Listener sub-class must be present.
This class presents the signatures of the functions which should be 
overwritten.
*/

//Defining Listener Sub-class: 
class SampleListener : public Listener {
    public:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
};

void SampleListener::onConnect(const Controller &controller)
{
    std::cout << "Connected\n" << std::endl;
}

void SampleListener::onFrame(const Controller& controller)
{
  const Frame frame = controller.frame();
  update_hands_position(frame, serial);
}

int main(int argc, char** argv) {
  
    SampleListener listener;
    Controller controller; //This object connects automatically to the
                           //Leap motion daemon. Tracking data can be 
                           //done by using the Controller::frame()
                           //method.

    if (!open_serial(serial)) {return 0;}
    
    std::cout << "Waiting for connection...\n";

    controller.addListener(listener); //Adding instance of Listener class.
   
    // Keep this process running until Enter is pressed.
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    // Remove the sample listener when done.
    controller.removeListener(listener);

    return 0;
}
