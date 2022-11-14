#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <mutex>
#include "Leap.h"

using namespace Leap;

/*
    #############
    # CONSTANTS #
    #############
*/

//Enumeration of 7 servos in robot-arm;
enum Servo { NONE = 0, BASE, LOWER_ARM, MIDDLE_ARM, HIGHER_ARM, ROTOR, GRABBER };

//Enumeration of possible Gestures to be detected.
enum CustomGesture { STILL = 0, UP, DOWN, SWIPE_LEFT, SWIPE_RIGHT, GRAB, RELEASE};

//Data structure used to return a combination of Gesture for a
//specific servo of the Raspberry Pi.
struct Movement {
  CustomGesture gesture;
  Servo servo;
};

const unsigned SIZE = 10; //Constant size to allow compilation of fixed arrays in C++.
const Vector EMPTY[10] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};

/*
    ####################
    # GLOBAL VARIABLES #
    ####################
*/

//These two arrays of vectors will store the 10 previous vectors of a
//given frame. This is used in order to compute gestures and catch
//actions. Here Vectors are Leap::Vector; not std::vector.
Vector rightHandPositions[10] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};
Vector leftHandPositions [10] = {Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector()};

//These variables are used to keep track of the indexes of such arrays
unsigned rightHandPosIndex = 0;
unsigned leftHandPosIndex = 0;

//Mutex needed because inner class is actually executed in different tread.
std::mutex movement_mutex;
std::mutex r_buffer_mutex;
std::mutex l_buffer_mutex;

/*
    ############################
    # GENERAL HELPER FUNCTIONS #
    ############################
*/

//This function returns true if a vector contains all 0s; it is
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

//This function prints an array of vectors to std::out.
void show_vec (Vector vec [])
{
  for (unsigned i = 0; i < SIZE; i++)
    std::cout << vec[i].y << " ";
  std::cout << "\n";
}

//This function prints Movement 'move' to std::out.
void show_movement (Movement move)
{
  movement_mutex.lock();
  
  switch(move.servo)
  { // NONE = 0, BASE, LOWER_ARM, MIDDLE_ARM, HIGHER_ARM, ROTOR, GRABBER
    case 1: std::cout <<  "BASE ";
    case 2: std::cout <<  "LOWER_ARM ";
    case 3: std::cout <<  "MIDDLE_ARM ";
    case 4: std::cout <<  "HIGHER_ARM ";
    case 5: std::cout <<  "ROTOR ";
    case 6: std::cout <<  "GRABBER ";
    default: std::cout << "NONE ";
  }
    
  switch (move.gesture)
  { // STILL = 0, UP, DOWN, SWIPE_LEFT, SWIPE_RIGHT, GRAB, RELEASE
    case 1: std::cout <<  "UP ";
    case 2: std::cout <<  "DOWN ";
    case 3: std::cout <<  "SWIPE_LEFT ";
    case 4: std::cout <<  "SWIPE_RIGHT ";
    case 5: std::cout <<  "GRAB ";
    case 6: std::cout <<  "RELEASE ";
    default: std::cout << "STILL ";
  }
    
  std::cout << std::endl;
  
  movement_mutex.unlock();
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
     const float OFFSET = 3.00;

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

//This function detects movement of the left hand and checks if
//movements GRAB and RELEASE are set.
CustomGesture detect_left_gesture(Hand hand, int fingers )
{
   //LEFT HAND IS USED FOR GRIPPING OR RELEASING THE GRABBER ONLY.
  CustomGesture gest = STILL;
  
  if (hand.isLeft() && !is_empty(leftHandPositions[SIZE-1]))
  { 
    switch (fingers)
    {
    case 5:  gest = RELEASE;
    case 0:  gest = GRAB;
    default: gest = STILL;
    }

    leftHandPosIndex = 0;
    copy_vec (EMPTY, leftHandPositions);
  }
  
  return gest;
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
    case 2 :
      return LOWER_ARM;

    case 3 :
      return MIDDLE_ARM;

    case 4:
      return HIGHER_ARM;
    
    default: return NONE;
  }
}

//This function starts processing data of the right hand.
void handle_right (Hand hand)
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

     //     show_movement(current_mov);
     
     r_buffer_mutex.unlock();
}

//This function starts processing data of the left hand.
void handle_left( Hand hand)
{
  Hand left = hand;
  FingerList l_fingers = left.fingers();
  int stretched_fingers = l_fingers.extended().count();
  Movement current_mov = {STILL, GRABBER};

  //Detect movement
  l_buffer_mutex.lock();

  if ( leftHandPosIndex < SIZE ){leftHandPositions[leftHandPosIndex++] = left.palmPosition();}
     
  else if (leftHandPosIndex == SIZE)
  {
    current_mov.gesture = detect_left_gesture(left, stretched_fingers);
  }

  show_movement(current_mov);
  
  l_buffer_mutex.unlock();
}

//This function assigns the correct hand to hands 'left' and 'right'
//and it sets some statistics of these hands.
void assign_hands(const Hand hand)
{
  if (hand.isLeft())
    handle_left(hand);
    
  if (hand.isRight())
    handle_right(hand);
}

//This function checks the amount of hands captured by the Leap Motion
//and stores in the vectors the positions of the detected hands.
void update_hands_position(const Frame &frame)
{
  int hands_amount = frame.hands().count();
  HandList hands = frame.hands();

  switch (hands_amount)
  {
     case 0:
       return;
       
     case 1:
     {
        assign_hands(hands[0]);
        break;
     }

     case 2:
     {
       assign_hands(hands[0]);
       assign_hands(hands[1]);
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
  update_hands_position(frame);
}

void set_up_controller (Controller &controller)
{
   set_up_configuration(controller);
    
   controller.enableGesture(Gesture::TYPE_SWIPE, true);
   controller.enableGesture(Gesture::TYPE_CIRCLE, true);
   controller.enableGesture(Gesture::TYPE_KEY_TAP, true);
    
}

int main(int argc, char** argv) {
    SampleListener listener;
    Controller controller; //This object connects automatically to the
                           //Leap motion daemon. Tracking data can be 
                           //done by using the Controller::frame()
                           //method.
    
    std::cout << "Waiting for connection...\n";

    set_up_controller(controller);
    controller.addListener(listener); //Adding instance of Listener class.
   
    // Keep this process running until Enter is pressed
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    // Remove the sample listener when done
    controller.removeListener(listener);
    
    return 0;
}
