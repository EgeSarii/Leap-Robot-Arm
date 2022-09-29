#include <unistd.h>
#include <iostream>
#include <string.h>
#include "Leap.h"

using namespace Leap;

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

*/

//In order to use Listeners, the Listener sub-class must be present.
//This class presents the signatures of the functions which should be 
//overwritten.

//Defining Listener Sub-class: 
class SampleListener : public Listener {
    public:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
};

void SampleListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
    const Frame frame = controller.frame();

    std::cout << "Frame id: " << frame.id() << "\n"
              << ", timestamp: " << frame.timestamp() << "\n"
              << ", hands: " << frame.hands().count() << "\n"
              << ", fingers: " << frame.fingers().count() << "\n"
              << "\n";

    sleep(2);
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
  //  bool acPowerSaver =  controller.config().getBool("power_saving_adapter");
  std::cout << "Enabling power saver... ";
  controller.config().setBool("power_saving_adapter", true);
  std::cout << "Power saver ENABLED!\n";

  //Parameter 2:
  //bool lowResourceMode =  controller.config().getBool("low_resource_mode_enabled");
  std::cout << "Enabling low resource mode... ";
  controller.config().setBool("low_resource_mode_enabled", true);
  std::cout << "mode ENABLED!\n";
  
  //Saving configurations.
  std::cout << "Configurations are saved.\n\n";
  controller.config().save();
}

int main(int argc, char** argv) {
    SampleListener listener;
    Controller controller; //This object connects automatically to the
                           //Leap motion daemon. Tracking data can be 
                           //done by using the Controller::frame()
                           // method.
    
    set_up_configuration(controller);
    
    controller.addListener(listener); //Adding instance of Listener class.

    // Keep this process running until Enter is pressed
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    // Remove the sample listener when done
    controller.removeListener(listener);
    return 0;
}

