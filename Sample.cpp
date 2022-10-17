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
const unsigned SIZE = 10; //Constant size to allow compilation of fixed arrays in C++.
const Vector EMPTY[10] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};

//ENUMERATION OF GESTURES (to be added)

/*
    ####################
    # GLOBAL VARIABLES #
    ####################
*/

//These two arrays of vectors will store the 10 previous vectors of a given
//frame. This is used in order to compute gestures and catch
//actions. Here Vectors are Leap::Vector.
Vector rightHandPositions[10] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};
Vector leftHandPositions [10] = {Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector()};

//These variables are used to keep track of the indexes of such arrays
unsigned rightHandPosIndex = 0;
unsigned leftHandPosIndex = 0;

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
void copy_vec (const Vector original [SIZE], Vector copy [SIZE])
{  
  for (unsigned i = 0; i < SIZE; i++)
    copy[i] = original[i];
}

//This function prints an array of vectors to std::out.
void show_vec (Vector vec [SIZE])
{
  for (unsigned i = 0; i < SIZE; i++)
    std::cout << vec[i].y << " ";
  std::cout << "\n";
}

//This function checks if the current vector has positions of the
//y-axis which increase. In this case the movement UP is detected.
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

//This function is used to detect and classify custom gestures.
void detect_custom_movements(Hand hand, int fingers )
{
  //RIGHT HAND IS USED FOR MOVEMENTS OF ROBOT
  if (hand.isRight() && !is_empty(rightHandPositions[SIZE-1]))
  {
    if (fingers == 5)
    {  
      if (check_y_direction(true)) {std::cout <<  "UP \n";}

      else if (check_y_direction(false)) {std::cout <<  "DOWN \n";}
    
      rightHandPosIndex = 0;
      copy_vec (EMPTY, rightHandPositions);
    }
    else {std::cout << "REPLACING RIGHT HAND\n";}
  }
}

//This function assigns the correct hand to hands 'left' and 'right'
//and it sets some statistics of these hands.
void assign_hands(const Hand hand, Hand &left, Hand &right, Vector
                  &right_pos, Vector &left_pos)
{
   if (hand.isLeft())
   {
     left = hand;
     FingerList l_fingers = hand.fingers();
     FingerList stretched_fingers = l_fingers.extended();

     leftHandPositions[leftHandPosIndex++] = left.palmPosition();
     leftHandPosIndex = leftHandPosIndex % SIZE;      //Circular buffer
     
     left_pos = left.palmPosition();
   }
    
   if (hand.isRight())
   {
     right = hand;
     FingerList r_fingers = hand.fingers();
     FingerList stretched_fingers = r_fingers.extended();

     std::mutex mutex; //Needed because inner class is actually executed in different tread.
     mutex.lock();

     if ( rightHandPosIndex < SIZE ){ rightHandPositions[rightHandPosIndex++] = right.palmPosition();}
     else if (rightHandPosIndex == SIZE){detect_custom_movements(right, stretched_fingers.count());}
     
     mutex.unlock();
     
   }
}

//This function checks the amount of hands captured by the Leap Motion
//and stores in the vectors the positions of the detected hands.
void update_hands_position(const Frame &frame, Vector &right_pos, Vector &left_pos)
{
  int hands_amount = frame.hands().count();
  HandList hands = frame.hands();
  Hand firstHand, secondHand, left, right;

  switch (hands_amount)
  {
     case 0:
       return;
       
     case 1:
       assign_hands(hands[0], left, right, right_pos, left_pos);
       break;

     case 2:
       assign_hands(hands[0], left, right, right_pos, left_pos);
       assign_hands(hands[1], left, right, right_pos, left_pos);
       break;
  }
}


//This function detects some gestures that are part of the Leap Motion
//API.

/*
   THIS SHOULD BE RETURN AN INT; WHICH CAN BE MAPPED TO A SPECIFIC
   ACTION IN THE ROBOT.
*/
void detect_movement(const Frame &frame)
{  
  GestureList gestures = frame.gestures();
  int32_t current_id;
    
  if (gestures.count() > 0) { current_id = gestures[0].id(); }
  
  for(Leap::GestureList::const_iterator gl = gestures.begin(); gl != frame.gestures().end(); gl++)
  {
    const int32_t next_id  = (*gl).id();
    std::cout << gestures.count() << "\n";
    
    if ( current_id != next_id )
    {
      switch ((*gl).type()) {
          case Gesture::TYPE_CIRCLE:
            //Handle circle gesture
	    std::cout << "CIRCLE!\n";
            break;
	  
          case Gesture::TYPE_KEY_TAP:
            //Handle key tap gestures
  	    std::cout << "TYPE KEY!\n";
            break;
	  
          case Gesture::TYPE_SWIPE:
            //Handle swipe gestures
	    std::cout << "SWIPE!\n" ;
            break;
	  
          default:
            //Handle unrecognized gestures
	    std::cout << "MOVEMENT NOT RECOGNIZED.\n";
            break;
       }
    }
      
    current_id = next_id;
  }
}

//This function sets up parameters for optimizing energy consumption.
void set_up_configuration(Controller controller){
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

  Vector right_pos, left_pos;
    
  update_hands_position(frame, right_pos, left_pos);

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
                           // method.
    
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
