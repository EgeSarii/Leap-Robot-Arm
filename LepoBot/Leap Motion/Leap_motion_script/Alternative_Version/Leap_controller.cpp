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

using namespace Leap;

/*
  ##########
  ## MAIN ##
  ##########
This cpp file contains the main function of the software part related
to the Leap Motion Controller.
*/

int main(int argc, char** argv)
{  
    SampleListener listener;
    Controller controller; //This object connects automatically to the
                           //Leap motion daemon. Tracking data can be 
                           //done by using the Controller::frame()
                           //method.

    if (!open_serial()) {return 0;}
    
    std::cout << "Waiting for connection...\n";

    controller.addListener(listener); //Adding instance of Listener class.
   
    // Keep this process running until Enter is pressed.
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    // Remove the sample listener when done.
    controller.removeListener(listener);

    return 0;
}
